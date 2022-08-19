#ifndef ASSISTANTPLUGINCONNECTOR_H
#define ASSISTANTPLUGINCONNECTOR_H

#include <QJsonDocument>
#include "plugininterface.h"
#include "speechrecognitor.h"

class KU_Assistant_PluginConnector : public KU::PLUGIN::PluginConnector
{
    Q_OBJECT
    Q_PROPERTY(QString configuration READ configuration NOTIFY configurationChanged)
    Q_PROPERTY(bool listening READ listening NOTIFY listeningChanged)
public:
    KU_Assistant_PluginConnector(QObject* parent = nullptr);
    void setup();

    virtual void pluginSlot(const QString& signal, const QVariantMap& data) override;

    Q_INVOKABLE void listen();
    Q_INVOKABLE void cancel();

    void processResult(QString const& result);

    QString configuration() const;
    bool    listening() const;

signals:
    void configurationChanged();
    void listeningChanged();

private:
    QString configurationLocation() const;

    QJsonDocument         _configuration;
    XB::SpeechRecognitor* _speechrecognitor;
};

#endif // ASSISTANTPLUGINCONNECTOR_H
