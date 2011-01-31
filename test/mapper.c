#include <stdio.h>
#include "../src/c/glq.h"
#include "../src/c/constants.h"
#include "../src/c/grav_tess.h"

int main()
{
    TESSEROID tess = {1000,44,46,-1,1,MEAN_EARTH_RADIUS-100000,MEAN_EARTH_RADIUS};
    double lon, lat, r = MEAN_EARTH_RADIUS + 1500000, res;
    GLQ *glqlon, *glqlat, *glqr;

    glqlon = glq_new(8, tess.w, tess.e);
    glqlat = glq_new(8, tess.s, tess.n);
    glqr = glq_new(8, tess.r1, tess.r2);
    
    for(lat = 20; lat <= 70; lat += 0.5)
    {
        for(lon = -25; lon <= 25; lon += 0.5)
        {
            res = tess_gzz(tess, lon, lat, r, *glqlon, *glqlat, *glqr);
            printf("%g %g %g\n", lon, lat, res);
        }
        printf("\n");
    }

    glq_free(glqlon);
    glq_free(glqlat);
    glq_free(glqr);
    
    return 0;
}