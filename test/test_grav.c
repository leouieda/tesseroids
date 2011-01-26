#include <stdio.h>
#include "../src/c/grav_sphere.h"
#include "../src/c/constants.h"

int main()
{
    double lon, lat, res;

    for(lat=0; lat<=0.5; lat+=0.01)
    {
        for(lon=0; lon<=0.5; lon+=0.01)
        {
            res = sphere_gzz(500.,10000.,0.25,0.25,MEAN_EARTH_RADIUS-10000.,lon,lat,MEAN_EARTH_RADIUS+1);
            printf("%g %g %g\n", lon, lat, res);

        }
        printf("\n");
    }
    return 0;
}