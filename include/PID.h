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
public:
    PID(/* args */);
    ~PID();
    void subjugationFunction ();
    void setCumulatedError (float);
    void setConsigne(float);
    void setError (float);
    float getConsigne ();
    float getError ();
    float getCumulatedError ();
    float getKp ();
    float getKi ();
    float getKd ();
};



#endif // PID_H
