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

float PID::getConsigne ()
{
  return m_Consigne;
}

void PID::setError (float _Error)
{
  m_Error = _Error;
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

void PID::subjugationFunction()
{
  ///Calcul de l'erreur
  hole_frequence = holeFrequence_test*CAPTOR_HOLES_NB;
  Counter_of_hole_frequence = (float)hole_frequence/(float)CAPTOR_HOLES_NB/29.f;
  Subjugation.setError( Subjugation.getConsigne() - Counter_of_hole_frequence);
  Subjugation.setCumulatedError (Subjugation.getCumulatedError() + Subjugation.getError());

  
}