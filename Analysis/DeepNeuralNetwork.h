#ifndef DEEPNEURALNETWORK_H
#define DEEPNEURALNETWORK_H

#include <QObject>
#include <opencv2/dnn.hpp>

class DeepNeuralNetwork : public QObject
{
    Q_OBJECT
public:
    explicit DeepNeuralNetwork(QObject *parent = nullptr);
    virtual ~DeepNeuralNetwork();

};

#endif // DEEPNEURALNETWORK_H
