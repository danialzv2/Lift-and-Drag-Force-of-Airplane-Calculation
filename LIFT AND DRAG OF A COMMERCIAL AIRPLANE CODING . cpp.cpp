#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

double safevmin, safevminkmh, FD, FD_kN, CD, energyrate, efficiency;

void weightcalculate (double &, double);
void areacalculate(double &, double, double);
void cruisingspeedconversion(double &);
void densityaltitude(double &);
void vmincalculate(double &, double, double);
void safevmincalculate(double &, double &, double);
void liftcoefficient (double &, double, double, double, double);
void dragcoefficient (double &, double);
void dragforce(double &, double, double, double, double);
void dragconversion(double &, double);
void powercalculate(double &, double, double);
void energyrequired(double &, double);
void displayoutput(double, double, double, double, double, double, double, double, double, double, double, double, double, double);

int main()
{
    double massplane, chordlength, wingspan, cruisevelocity, weight, area, density, vmin, CL, power;
    char sentinel = 'Y';

    do
    {
        cout<< "Lift and Drag of a Commercial Airplane Simulator\n\n";

        cout<< "Please enter mass of the plane (kg): ";
        cin>> massplane;

        cout<< "Please enter the chord length of the plane wing (m): ";
        cin>> chordlength;

        cout<< "Please enter the wingspan of the wing (m): ";
        cin>> wingspan;

        cout<< "Please enter the cruising velocity of the plane (km/h): ";
        cin>> cruisevelocity;


        weightcalculate(weight, massplane);
        areacalculate(area, chordlength, wingspan);
        cruisingspeedconversion(cruisevelocity);
        densityaltitude(density);
        vmincalculate(vmin, weight, area);
        liftcoefficient(CL, weight, density, cruisevelocity, area);
        dragforce(FD, CD, area, density, cruisevelocity);
        powercalculate(power, FD_kN, cruisevelocity);
        displayoutput(weight, area, density, cruisevelocity, vmin, safevmin, safevminkmh, CL, CD, FD, FD_kN, power, efficiency, energyrate);


        cout<< "\n\nDo you wish to continue?\nPress Y for yes and N for no"<< endl;
        cin>> sentinel;

        if(sentinel == 'Y')
        {
            cout<< "\n\n";
            cout<< "_________________________________________________________________________________________________________"<< endl;
        }

    } while (sentinel == 'Y');

    return 0;
}

void weightcalculate (double &weight, double mass)
{
    double g = 9.81;
    weight = mass * g;
}

void areacalculate (double &area, double chord, double span)
{
    area = chord * span;
}

void cruisingspeedconversion(double &cruisevelocity)
{
    cruisevelocity = cruisevelocity / 3.6;
}

void densityaltitude(double &density)
{
    double cruisealtitude;
    double altitude[12][2] = {3000,0.909,4000,0.891,5000,0.872,6000,0.660,7000,0.590,8000,0.526,9000,0.467,10000,0.414,11000,0.350,12000,0.312,14000,0.228,16000,0.166};

    do
    {
        cout<< "Please enter the cruising altitude of the plane (m): ";
        cin>> cruisealtitude;

        if(cruisealtitude > 16000)
        {
            cout<< "Altitude is too high, please reenter the altitude."<< endl;
        }
        else if(cruisealtitude < 3000)
        {
            cout<< "Altitude is too low, please reenter the altitude."<< endl;
        }
        else if(cruisealtitude >= 3000 && cruisealtitude <= 16000)
        {
            for(int i = 0; i < 12; i++)
            {
                if(cruisealtitude >= altitude[i][0] && cruisealtitude < altitude[i+1][0])
                {
                    density = altitude[i][1];
                }
                else if(cruisealtitude == 16000)
                {
                    density = altitude[i][1];
                }
            }
        }
    }while (cruisealtitude > 16000 || cruisealtitude < 3000);
}

void vmincalculate(double &vmin, double weight, double area)
{
    int flaptype;
    double CL, grounddensity = 1.2;

    do
    {
        cout<< "Please enter the type of flap used for take off"<< endl;
        cout<< "1. No Flap"<< endl;
        cout<< "2. Single Flap"<< endl;
        cout<< "3. Double Flap"<< endl;
        cin>> flaptype;

        switch(flaptype)
        {
            case 1:
                {
                    CL = 1.52;
                    break;
                }
            case 2:
                {
                    CL = 2.67;
                    break;
                }
            case 3:
                {
                    CL = 3.48;
                    break;
                }
            default:
                {
                    cout<< "Error. Invalid flap type. Please try again."<< endl;
                }
        }

    }while (flaptype > 3 || flaptype < 1);

    vmin = sqrt((2*weight)/(grounddensity*CL*area));

    safevmincalculate(safevmin, safevminkmh, vmin);
}

void safevmincalculate(double &safevmin, double &safevminkmh, double vmin)
{
    safevmin = 1.2 * vmin;
    safevminkmh = safevmin * 3.6;
}

void liftcoefficient (double &CL, double weight, double density, double velocity, double area)
{
    CL = weight /(0.5*density*pow(velocity,2)*area);

    dragcoefficient(CD, CL);
}

void dragcoefficient(double &CD, double CL)
{
    double CD_range[6][2] = {0, 0.01, 1.5, 0.06, 1.6, 0.07, 2.6, 0.12, 2.7, 0.13, 3.5, 0.3};

    for(int i = 0; i < 6; i++)
    {
        if(CL >= CD_range[i][0] && CL <= CD_range[i+1][0])
        {
            CD = (CD_range[i][1] + CD_range[i+1][1]) / 2;
        }
        else if(CL > 3.5)
        {
            CD = CD_range[i][1];
        }
    }
}

void dragforce(double &FD, double CD, double area, double density, double velocity)
{
    FD = 0.5 * CD * area * density * pow(velocity, 2);

    dragconversion(FD_kN, FD);
}

void dragconversion(double &FD_kN, double FD)
{
    FD_kN = FD / 1000;
}

void powercalculate(double &power, double FD, double velocity)
{
    power = FD * velocity;

    energyrequired(energyrate, power);
}

void energyrequired(double &energy, double power)
{
    double engineefficiency[5][2] = {1, 0.85, 2, 0.3, 3, 0.25, 4, 0.75, 5, 0.4};
    int enginetype;

    do
    {
        cout<< "Please enter the type of the airplane engine"<< endl;
        cout<< "1. Turboprop Engine"<< endl;
        cout<< "2. Turbojet Engine"<< endl;
        cout<< "3. Turboshaft Engine"<< endl;
        cout<< "4. Turbofan Engine"<< endl;
        cout<< "5. Ramjet Engine"<< endl;
        cin>> enginetype;

        if(enginetype < 1 || enginetype > 5)
        {
            cout<< "Error. Invalid engine type. Please try again"<< endl;
        }
    }while (enginetype < 1 || enginetype > 5);

    for(int i = 0; i < 5; i++)
    {
        if(enginetype == engineefficiency[i][0])
        {
            efficiency = engineefficiency[i][1];
        }
    }

    energy = power / efficiency;
}

void displayoutput(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, double k, double l, double m, double n)
{
    cout<< "___________________________________________________________________________________________________"<< endl;
    cout<< "Final Results:"<< endl;
    cout<< "The weight of the airplane is "<< a<< " N"<< endl;
    cout<< "The area of the wing of the airplane is "<< b<< " m^2"<< endl;
    cout<< "The density of the air at cruising altitude is "<< c<< " kg/m^3"<< endl;
    cout<< "The cruising velocity of the airplane is "<< d<< " m/s"<< endl;
    cout<< "The minimum take off speed for the airplane is "<< e<< " m/s"<< endl;
    cout<< "The safe minimum take off speed for the airplane is "<< f<< " m/s or "<< g<< " km/h"<< endl;
    cout<< "The lift coefficient of the airplane when cruising steadily is "<< h<< endl;
    cout<< "The drag coefficient of the airplane when cruising steadily is "<< i<< endl;
    cout<< "The drag force acting on the airplane when cruising steadily is "<< j<< " N or "<< k<< " kN"<< endl;
    cout<< "The power required to overcome the drag force is "<< l<< " kW"<< endl;
    cout<< "The efficiency of the aircraft engine is "<< m * 100<< " %"<< endl;
    cout<< "The plane requires energy input at a rate of "<< n<< " kJ/s"<< endl;
}
