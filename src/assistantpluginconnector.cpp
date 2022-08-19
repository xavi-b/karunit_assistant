#include "assistantpluginconnector.h"
#include "settings.h"

#include <QFileInfo>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>

KU_Assistant_PluginConnector::KU_Assistant_PluginConnector(QObject* parent)
    : KU::PLUGIN::PluginConnector(parent)
{
    _speechrecognitor = new XB::SpeechRecognitor(this);
}

void KU_Assistant_PluginConnector::setup()
{
    connect(_speechrecognitor, &XB::SpeechRecognitor::resultString, this, &KU_Assistant_PluginConnector::processResult);
    connect(_speechrecognitor, &XB::SpeechRecognitor::stateChanged, this, &KU_Assistant_PluginConnector::listeningChanged);

    QFile f(configurationLocation());

    bool exists = f.exists();

    emitLogSignal(configurationLocation() + ' ' + QString::number(exists));

    if (f.open(QIODevice::ReadWrite))
    {
        if (!exists)
        {
            QFile defaultConfig(":/karunit_assistant/default.json");
            if (defaultConfig.open(QIODevice::ReadOnly))
                f.write(defaultConfig.readAll());
            f.seek(0);
        }

        _configuration = QJsonDocument::fromJson(f.readAll());
        emit configurationChanged();
    }
}

void KU_Assistant_PluginConnector::pluginSlot(QString const& signal, QVariantMap const& data)
{
    if (signal == "currentLocaleChanged")
        _speechrecognitor->setLang(data["locale"].toString().split("_").first());
}

void KU_Assistant_PluginConnector::listen()
{
    _speechrecognitor->record();
}

void KU_Assistant_PluginConnector::cancel()
{
    _speechrecognitor->cancel();
}

void KU_Assistant_PluginConnector::processResult(const QString& result)
{
    emitLogSignal("RESULT: " + result);

    QStringList results = result.split(' ');

    QJsonArray commands = _configuration.object()["commands"].toArray();

    QJsonObject closestCommand;
    QJsonArray  closestKeywords;

    auto isCloser = [](QJsonArray const& keywords, QJsonArray const& closestKeywords, QStringList const& results) {
        auto listStartsWith = [](QStringList const& results, QJsonArray const& keywords) {
            if (keywords.size() == 0 || results.size() == 0 || results.size() < keywords.size())
                return false;

            for (int i = 0; i < keywords.size(); ++i)
            {
                if (keywords.at(i).toString() != results.at(i))
                    return false;
            }

            return true;
        };

        if (listStartsWith(results, keywords))
            return keywords.size() > closestKeywords.size();

        return false;
    };

    for (auto const& command : commands)
    {
        QJsonArray keywordsList = command.toObject()["keywords"].toArray();

        for (auto const& keywords : keywordsList)
        {
            if (isCloser(keywords.toArray(), closestKeywords, results))
            {
                closestCommand  = command.toObject();
                closestKeywords = keywords.toArray();
            }
        }
    }

    if (!closestCommand.isEmpty())
    {
        emitLogSignal("!closestCommand.isEmpty()");

        auto clearKeywordsFromResult = [](QString const& result, QJsonArray const& keywords) {
            QString r = result;

            for (int i = 0; i < keywords.size(); ++i)
            {
                QString keyword = keywords.at(i).toString();
                r.remove(0, keyword.size());
                r = r.trimmed();
            }

            return r;
        };

        if (!closestCommand["show_data"].toString().isEmpty())
        {
            emitLogSignal("show_data");
            emitShowPluginSignal(closestCommand["show_data"].toString());
        }

        QVariantMap data;

        if (!closestCommand["permanent_data"].toArray().isEmpty())
        {
            emitLogSignal("permanent_data");
            for (auto const& permanent_data : closestCommand["permanent_data"].toArray())
            {
                data.insert(permanent_data.toObject().toVariantMap());
            }
        }

        if (!closestCommand["result_data"].toString().isEmpty())
        {
            emitLogSignal("result_data");
            data[closestCommand["result_data"].toString()] = clearKeywordsFromResult(result, closestKeywords);
        }

        emit pluginDataSignal(closestCommand["signal"].toString(), data);
    }
}

QString KU_Assistant_PluginConnector::configuration() const
{
    return _configuration.toJson(QJsonDocument::JsonFormat::Indented);
}

bool KU_Assistant_PluginConnector::listening() const
{
    return _speechrecognitor->state() == QMediaRecorder::RecordingState;
}

QString KU_Assistant_PluginConnector::configurationLocation() const
{
    return QFileInfo(KU::Settings::instance()->fileName()).dir().filePath("assistant.json");
}
