#include "CrossingController.h"

CrossingController::CrossingController(
    TrainDetector& detector,
    SignalController& signalController,
    CommunicationHandler& comm,
    unsigned long safetyMargin
)
  : _detector(detector),
    _signal(signalController),
    _comm(comm),
    _safetyMargin(safetyMargin),
    _startTime(0),
    _trainId(-1) {}

void CrossingController::begin() {
  _detector.begin();
  _signal.begin();
}

void CrossingController::update() {
  _detector.update();

  unsigned long now = millis();
  unsigned long elapsed = now - _startTime;
  unsigned long predictedTime = _detector.getPredictedTime();

  unsigned long warningStart = (predictedTime * 1000 > _safetyMargin)
      ? predictedTime * 1000 - _safetyMargin
      : 0;

  if (_detector.firstTriggered()) {
    _comm.createTrain(_trainId);
    _startTime = now;
  }

  if (_detector.secondTriggered()) {
    predictedTime = _detector.getPredictedTime();
    _comm.sendPrediction(_trainId, predictedTime);
  }

  if (elapsed > warningStart && elapsed < predictedTime * 1000) {
    _signal.setState(SIGNAL_WARNING);
  }

  if (_detector.trainPassed()) {
    _comm.sendCrossed(_trainId);
    _trainId = -1;
    _signal.setState(SIGNAL_IDLE);
  }

  _signal.update();
}