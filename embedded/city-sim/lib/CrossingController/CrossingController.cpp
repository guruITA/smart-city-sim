#include "CrossingController.h"

CrossingController::CrossingController(TrainDetector detector,
                                       SignalController signalController,
                                       CommunicationHandler comm)
  : _detector(detector), _signal(signalController), _comm(comm), _trainId(-1) {}

void CrossingController::begin() {
  _detector.begin();
  _signal.begin();
}

void CrossingController::update() {
  _detector.update();

  if (_detector.firstTriggered()) {
    if (_comm.createTrain(_trainId)) {
      Serial.println("Train created");
    }
  }

  if (_detector.secondTriggered()) {
    float t = _detector.getPredictedTime();
    _comm.sendPrediction(_trainId, t);
    _signal.setState(SIGNAL_WARNING);
  }

  if (_detector.trainPassed()) {
    _comm.sendCrossed(_trainId);
    _signal.setState(SIGNAL_IDLE);
    _trainId = -1;
  }

  _signal.update();
}