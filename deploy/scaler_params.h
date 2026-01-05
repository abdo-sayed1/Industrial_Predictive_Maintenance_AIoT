#ifndef SCALER_PARAMS_H
#define SCALER_PARAMS_H

// Scaler parameters from Python training
const float scaler_mean[9] = {
  0.0599141163,   // ax
  0.0468072370,   // ay
  0.0421709463,   // az
  0.277288616,    // gx
  0.262001281,    // gy
  0.281187325,    // gz
  43.4209505,     // temp
  2.16994154,     // current
  1476.02631      // rpm
};

const float scaler_std[9] = {
  0.12853713,     // ax
  0.1281364,      // ay
  0.12954736,     // az
  1.33143455,     // gx
  1.31496109,     // gy
  1.33515316,     // gz
  6.59782181,     // temp
  0.43492526,     // current
  58.00813606     // rpm
};

#endif

