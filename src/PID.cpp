/**
 * @file PID.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "PID.h"

/**
 * @brief constructor of PID
 * 
 */
PID::PID(/* args */)
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
float PID::getError ()
{
  return m_Error;
}

/**
 * @brief getter of last error
 * 
 * @return float 
 */
float PID::getLastError ()
{
  return m_LastError;
}

/**
 * @brief getter of consigne
 * 
 * @return float 
 */
float PID::getConsigne ()
{
  return m_Consigne;
}

/**
 * @brief Setter of error
 * 
 * @param _Error 
 */
void PID::setError (float _Error)
{
  m_Error = _Error;
}

/**
 * @brief Setter of last error
 * 
 * @param _Error 
 */
void PID::setLastError (float _Error)
{
  m_LastError = _Error;
}

/**
 * @brief Setter of cumulated
 * 
 * @param _CumulatedError 
 */
void PID::setCumulatedError (float _CumulatedError)
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
float PID::getCumulatedError ()
{
    return m_Cumulated_Error;
}

/**
 * @brief Getter of Kp
 * 
 * @return float 
 */
float PID::getKp ()
{
  return m_Kp;
}

/**
 * @brief Getter of Ki
 * 
 * @return float 
 */
float PID::getKi ()
{
  return m_Ki;
}

/**
 * @brief Getter of Kd
 * 
 * @return float 
 */
float PID::getKd ()
{
  return m_Kd;
}

/**
 * @brief Function to use the PID Corrector
 * 
 * @param state state of the motor
 * @param speed speed of the motor
 * @param wanted_speed the speed we aimed
 * @return float 
 */
float PID::subjugationFunction(uint8_t state,float speed,float wanted_speed)
{
  if(state == ON )
  {
      //Error calcul
      setConsigne(wanted_speed);
      setError( getConsigne() - speed);
      setCumulatedError ( getCumulatedError() +  getError());
      setLastError( getError());

      //PID corrector
      return  getKp() *  getError() +  getKi() *  getCumulatedError() +  getKd() * ( getError() -  getLastError() );  
  }
  setError(0);
  setCumulatedError(0);
  setLastError(0);
  return 0;
}