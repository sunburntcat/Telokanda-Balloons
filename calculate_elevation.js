// This code is meant to be used to add a zElevation variable
//    to the Catena 4610 decoded JSON output
// It uses the meteorological hypsometric equation to derive
//    atmospheric height by integrating pressure, dew point, and
//    temperature at various levels.
// 
// For reference:
//     https://en.wikipedia.org/wiki/Hypsometric_equation
//
// Instructions:
//    Copy and paste this code into a node-red "function" node,
//      and provide it with Catena 4610 outputs

// Set initial values
p = msg.payload.p;
tDewC = msg.payload.tDewC;
t = msg.payload.tempC + 273.15; // Convert temperature to Kelvin

// Calculate actual vapor pressure
e = 6.112*Math.pow(10,(7.5*tDewC)/(237.7+tDewC));

// Calculate mixing ratio
w = 0.62197 * ( e / (p-e));

// Calculate Tv in kelvin
tv2 = (1+(0.61*w))*t;

// Check that the first observation exists
if (context.get(msg.devid+'.z1')||0)
{
    // Get previous layers' values
    p1 = context.get(msg.devid+'.p1');
    tv1 = context.get(msg.devid+'.tv1');
    z1 = context.get(msg.devid+'.z1');
    
    // Approximate average virtual temperature of layer
    tv = tv1 + (tv2 - tv1)/2.0;

    // Do thickness calculation
    R = 287.058; // specific gas constant dry air
    g = 9.80664; // gravitational acceleration
    z2 = (R*tv/g)*Math.log(p1/p) + z1;
    
} else { // First field doesn't exist

    z2 = 1.0 // 1 meter above the surface
}

// Store values for next iteration
context.set(msg.devid+'.p1', p);
context.set(msg.devid+'.tv1', tv2);
context.set(msg.devid+'.z1', z2);

msg.payload.zElevation = z2;
return msg;
