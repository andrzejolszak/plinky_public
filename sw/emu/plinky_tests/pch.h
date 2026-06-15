// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

#endif //PCH_H

#define _CRT_SECURE_NO_WARNINGS

enum EParams {

  P_SHAPE,
  P_DISTORTION,
  P_PITCH,
  P_OCTAVE,
  P_GLIDE,
  P_OSC_INTERVAL,

  P_NOISE,
  P_RESONANCE,
  P_DEGREE,
  P_SCALE,
  P_MICROTONE,
  P_COLUMN,

  P_SENSITIVITY,
  P_ATTACK,
  P_DECAY,
  P_SUSTAIN,
  P_RELEASE,
  P_ENV1_UNUSED,

  P_ENV2_LEVEL,
  P_ATTACK2,
  P_DECAY2,
  P_SUSTAIN2,
  P_RELEASE2,
  P_ENV2_UNUSED,

  P_DELAY,
  P_DL_TIME,
  P_PINGPONG,
  P_DL_WOBBLE,
  P_FEEDBACK,
  P_TEMPO,

  P_REVERB,
  P_RV_TIME,
  P_SHIMMER,
  P_RV_WOBBLE,
  P_RVUNUSED,
  P_SWING,

  P_ARP,
  P_ORDER,
  P_ARP_CLOCK_DIV,
  P_ARP_CHANCE,
  P_ARP_EUCLID_LEN,
  P_ARP_OCTAVES,

  P_LATCH,
  P_SEQ_ORDER,
  P_SEQ_CLOCK_DIV,
  P_SEQ_CHANCE,
  P_SEQ_EUCLID_LEN,
  P_GATE_LEN,

  P_SCRUB,
  P_GRAIN_SIZE,
  P_PLAY_SPEED,
  P_TIME_STRETCH,
  P_SAMPLE,
  P_PATTERN,

  P_JIT_SCRUB,
  P_JIT_GRAIN_SIZE,
  P_JIT_PLAY_SPEED,
  P_JIT_TIME_STRETCH,
  P_JIT_SAMPLE,
  P_STEP_OFFSET,

  P_A_CV_LEVEL,
  P_A_OFFSET,
  P_A_DEPTH,
  P_A_RATE,
  P_A_SHAPE,
  P_A_SYMMETRY,

  P_B_CV_LEVEL,
  P_B_OFFSET,
  P_B_DEPTH,
  P_B_RATE,
  P_B_SHAPE,
  P_B_SYMMETRY,

  P_X_CV_LEVEL,
  P_X_OFFSET,
  P_X_DEPTH,
  P_X_RATE,
  P_X_SHAPE,
  P_X_SYMMETRY,

  P_Y_CV_LEVEL,
  P_Y_OFFSET,
  P_Y_DEPTH,
  P_Y_RATE,
  P_Y_SHAPE,
  P_Y_SYMMETRY,

  P_SYNTH,
  P_WET_DRY,
  P_HPF,
  P_MIDI_CH_IN,
  P_SETTINGS,
  P_VOLUME,

  P_INPUT,
  P_INPUT_WET_DRY,
  P_SYS_UNUSED1,
  P_MIDI_CH_OUT,
  P_ACCEL_SENS,
  P_MIX_WIDTH,

  P_SOUND = 96,
  P_ENV,
  P_FX,
  P_SEQ_ARP,
  P_SAMPLER,
  P_AB_CV_LFO,
  P_XY_CV_LFO,
  P_MIXER,

  P_FADERS = 104,
  P_SLOPE,
  P_TOUCH,
  P_INPUT_A,
  P_INPUT_B,
  P_INPUT_X,
  P_INPUT_Y,
  P_RANDOM,

  P_SHIFT_UP = 112,
  P_SHIFT_DOWN,
  P_PRESET,
  P_BACK,
  P_FWD,
  P_X,
  P_REC,
  P_PLAY
};