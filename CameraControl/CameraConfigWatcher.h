#ifndef CAMERA_CONFIG_WATCHER_H
#define CAMERA_CONFIG_WATCHER_H

#include <QObject>
#include <QScopedPointer>

class CameraConfigWatcher : public QObject
{
private:
    Q_OBJECT

    struct privateStruct;

public:
    explicit CameraConfigWatcher(QObject *parent = nullptr);
    virtual ~CameraConfigWatcher();

    void addWatchFile(QString fileName);

signals:
    void onConfigFileChanged(QString fileName);

private slots:
    void onFileChanged(QString path);

private:
    QScopedPointer<privateStruct> d;
};

#endif // CAMERACONFIGWATCHER_H
