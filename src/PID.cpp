#include "PID.h"


PID::PID(/* args */)
{
}

PID::~PID()
{
}

float PID::getError ()
{
  return m_Error;
}

float PID::getLastError ()
{
  return m_LastError;
}

float PID::getConsigne ()
{
  return m_Consigne;
}

void PID::setError (float _Error)
{
  m_Error = _Error;
}

void PID::setLastError (float _Error)
{
  m_LastError = _Error;
}

void PID::setCumulatedError (float _CumulatedError)
{
    m_Cumulated_Error = _CumulatedError;
}
void PID::setConsigne(float _Consigne)
{
    m_Consigne = _Consigne;
}

float PID::getCumulatedError ()
{
    return m_Cumulated_Error;
}

float PID::getKp ()
{
  return m_Kp;
}

float PID::getKi ()
{
  return m_Ki;
}

float PID::getKd ()
{
  return m_Kd;
}

void PID::subjugationFunction()
{
  ///Calcul de l'erreur
  hole_frequence = holeFrequence_test*CAPTOR_HOLES_NB;
  Counter_of_hole_frequence = (float)hole_frequence/(float)CAPTOR_HOLES_NB/29.f;
  Subjugation.setError( Subjugation.getConsigne() - Counter_of_hole_frequence);
  Subjugation.setCumulatedError (Subjugation.getCumulatedError() + Subjugation.getError());
  Subjugation.setLastError(Subjugation.getError());

  ///Regulateur P
  motorA->speed = Subjugation.getKp() * Subjugation.getError();
  motorB->speed = Subjugation.getKp() * Subjugation.getError();

  ///Regulateur PI 
  motorA->speed = Subjugation.getKp() * Subjugation.getError() + Subjugation.getKi() * Subjugation.getCumulatedError() ;
  motorB->speed = Subjugation.getKp() * Subjugation.getError() + Subjugation.getKi() * Subjugation.getCumulatedError() ;
  
  ///regulateur PID
  motorA->speed = Subjugation.getKp() * Subjugation.getError() + Subjugation.getKi() * Subjugation.getCumulatedError() + Subjugation.getKd() * (Subjugation.getError() ,Subjugation.getLastError() );
  motorB->speed = Subjugation.getKp() * Subjugation.getError() + Subjugation.getKi() * Subjugation.getCumulatedError() + Subjugation.getKd() * (Subjugation.getError() ,Subjugation.getLastError() );
   
}