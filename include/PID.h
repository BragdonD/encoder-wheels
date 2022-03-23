#if !defined(PID_H)
#define PID_H

#include "motor.h"
class PID
{
private:
    float m_Error = 0;
    float m_Consigne = 0;
    float m_Cumulated_Error = 0;
    float m_Kp = 0;
    float m_Ki = 0;
    float m_Kd = 0;
    float m_LastError = 0;
public:
    PID(float,float,float);
    ~PID();
    float subjugationFunction (uint8_t,float,float);
    void setCumulatedError (float);
    void setConsigne(float);
    void setError (float);
    void setLastError (float);
    float getConsigne ();
    float getError ();
    float getLastError ();
    float getCumulatedError ();
    float getKp ();
    float getKi ();
    float getKd ();
};

#endif // PID_H
