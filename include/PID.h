#if !defined(PID_H)
#define PID_H

#include <SimpleTimer.h>
#include "global.h"
#include "robot.h"



class PID
{
private:
    float m_Error;
    float m_Consigne;
    float m_Cumulated_Error;
    float m_Kp;
    float m_Ki;
    float m_Kd;
    float m_LastError;
public:
    PID(/* args */);
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

extern PID Subjugation;



#endif // PID_H
