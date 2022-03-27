/**
 * @file PID.cpp
 * @author DUCLOS Thomaas - KEBE Ibrahim - VIDAL Hugo - FEVE Quentin
 * @brief
 * @version 0.1
 * @date 2022-03-23
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "PID.h"

/**
 * @brief Construct a new PID::PID object
 * 
 */
PID::PID() : m_Kp(0), m_Ki(0), m_Kd(0) {}

/**
 * @brief Construct a new PID::PID object
 *
 * @param _kp
 * @param _ki
 * @param _kd
 */

[[deprecated("Cannot be used in the last part of the project because we need to declare PID object inside header global.h as extern")]]
PID::PID(float _kp, float _ki, float _kd) : m_Kp(_kp), m_Ki(_ki), m_Kd(_kd)
{
}

/**
 * @brief Destroy the PID::PID object
 *
 */
PID::~PID()
{
}

/**
 * @brief getter of error
 *
 * @return float
 */
float PID::getError()
{
  return m_Error;
}

/**
 * @brief getter of last error
 *
 * @return float
 */
float PID::getLastError()
{
  return m_LastError;
}

/**
 * @brief getter of consigne
 *
 * @return float
 */
float PID::getConsigne()
{
  return m_Consigne;
}

/**
 * @brief Setter of error
 *
 * @param _Error
 */
void PID::setError(float _Error)
{
  m_Error = _Error;
}

/**
 * @brief Setter of last error
 *
 * @param _Error
 */
void PID::setLastError(float _Error)
{
  m_LastError = _Error;
}

/**
 * @brief Setter of cumulated
 *
 * @param _CumulatedError
 */
void PID::setCumulatedError(float _CumulatedError)
{
  m_Cumulated_Error = _CumulatedError;
}

/**
 * @brief Setter of consigne
 *
 * @param _Consigne
 */
void PID::setConsigne(float _Consigne)
{
  m_Consigne = _Consigne;
}

/**
 * @brief Getter of cumulated error
 *
 * @return float
 */
float PID::getCumulatedError()
{
  return m_Cumulated_Error;
}

/**
 * @brief Getter of Kp
 *
 * @return float
 */
float PID::getKp()
{
  return m_Kp;
}

/**
 * @brief Getter of Ki
 *
 * @return float
 */
float PID::getKi()
{
  return m_Ki;
}

/**
 * @brief Getter of Kd
 *
 * @return float
 */
float PID::getKd()
{
  return m_Kd;
}

void PID::setKp(float value)
{
  m_Kp = value;
}
void PID::setKi(float value)
{
  m_Ki = value;
}
void PID::setKd(float value)
{
  m_Kd = value;
}

/**
 * @brief Function to use the PID Corrector
 *
 * @param state state of the motor
 * @param speed speed of the motor
 * @param wanted_speed the speed we aimed
 * @return float
 */
float PID::subjugationFunction(uint8_t state, float speed, float wanted_speed)
{
  if (state == ON)
  {
    // Error calcul
    setConsigne(wanted_speed);
    setLastError(getError());
    setError(getConsigne() - speed);
    setCumulatedError(getCumulatedError() + getError());
    if (getCumulatedError() > 2.0)
      setCumulatedError(2.0);
    if (getCumulatedError() < -2.0)
      setCumulatedError(-2.0);

#if DEBUG
    Serial.printf("Wanted Speed : %f\n", wanted_speed);
    Serial.printf("Speed : %f\n", speed);
    Serial.printf("Error: %f\n", getError());
    Serial.printf("Cumlated Error: %f\n", getCumulatedError());
    Serial.printf("Last Error : %f\n", getLastError());
    Serial.printf("New Speed : %f\n", ((getKp() * getError()) + (getKi() * getCumulatedError()) + (getKd() * (getError() - getLastError()))));
    Serial.printf("%f\n", getKp() * getError());
    Serial.printf("%f\n", getKi() * getCumulatedError());
    Serial.printf("%f\n", getKd() * (getError() - getLastError()));
    Serial.printf("actual speed A : %f\n\n", ((getKp() * getError()) + (getKi() * getCumulatedError()) + (getKd() * (getError() - getLastError()))));
#endif

    // PID corrector
    return ((getKp() * getError()) + (getKi() * getCumulatedError()) + (getKd() * (getError() - getLastError())));
  }
  setError(0);
  setCumulatedError(0);
  setLastError(0);
  return 0;
}