#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef WASM
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

#define BLOCK_SAMPLES 64
extern "C" {

typedef unsigned char u8;
typedef unsigned int u32;
typedef short s16;
typedef signed char s8;

// Display pixels
extern uint32_t emupixels[128 * 32];

extern int16_t accel_raw[3];
extern float accel_lpf[2];
extern float accel_smooth[2];

extern unsigned short expander_out[4];

void resetspistate(void);

/// <summary>
/// 
/// </summary>
/// <param name="araw">A pot: 0 - 1</param>
/// <param name="braw">B pot: 0 - 1</param>
/// <param name="pitchcv">Pitchcv in semitones: -1.5*12 - 6*12</param>
/// <param name="gatecv">0 - 5</param>
/// <param name="xcv">-5 - 5</param>
/// <param name="ycv">-5 - 5</param>
/// <param name="acv">-5 - 5</param>
/// <param name="bcv">-5 - 5</param>
/// <param name="gateforce">Bool 0/1</param>
/// <param name="pitchsense">0 = no pitch plug, 1 = pitch loopback (in only), 2 = pitch loopback (cabled! implies pitchcv = emupitchloopback * 12.f), 3 = pitch control</param>
/// <param name="gatesense">Bool 0/1</param>
void emu_setadc(float araw, float braw, float pitchcv, float gatecv, float xcv, float ycv, float acv, float bcv, int gateforce,
                int pitchsense, int gatesense);

void plinky_frame();
void plinky_init();
void uitick(u32 *dst, const u32 *src, int half);

/// <summary>
/// Gain history
/// </summary>
extern float gainhistoryrms[512];
extern int ghi;
extern float knobhistory[512];
extern int khi;
typedef unsigned short u16;

// Encoder value EMU
static float encraw = 0.f;

/// <summary>
/// The encoder value. Speed/delta semantics, comes back to ~2 when not moving
/// </summary>
extern volatile int encval;

/// <summary>
/// The encoder buttong bool 0/1
/// </summary>
extern volatile u8 encbtn;

// A and B knobs EMU
static float araw = 0.5f;
static float braw = 0.5f;

extern float arpdebug[1024];
extern int arpdebugi;


/// <summary>
/// CV outs
/// 0 - trigger
/// 1 - clock
/// 2 - pressure
/// 3 - gate
/// 4 - pitchlo
/// 5 - pitchhi
/// </summary>
extern float emucvout[6][256];

/// <summary>
/// CV out index
/// </summary>
extern int emucvouthist;

extern float emupitchloopback;
extern int emupitchsense, emugatesense;
static bool shutdownNow;
static bool enable_emu_audio;

void ApplyUF2File(const char *fname);

extern int samplelen;

typedef struct FingerRecord {
  u8 pos[4];
  u8 pressure[8];
} FingerRecord;

typedef struct Preset {
  s16 params[96][8];
  u8 arpon;
  s8 loopstart_step;
  s8 looplen_step;
  u8 paddy[16 - 3];
} Preset;

typedef struct PatternQuarter {
  FingerRecord steps[16][8];
  s8 autoknob[16 * 8][2];
} PatternQuarter;

extern Preset rampreset;
extern PatternQuarter rampattern[4];
extern s8 cur_step;

// Button LEDs
extern u8 emuleds[9][8];
extern short _flashram[8 * 2 * 1024 * 1024];

// Touch input
extern int emutouch[9][2];

extern "C" float life_damping;
extern "C" float life_force;
extern "C" float p_grainpos;
extern "C" float p_grainsize;
extern "C" float p_timestretch;
extern "C" float p_pitchy;
extern "C" int64_t p_playhead;

/// <summary>
/// For monitoring levels/gain staging
/// </summary>
extern "C" float m_compressor, m_dry, m_audioin, m_dry2wet, m_delaysend, m_delayreturn, m_reverbin, m_reverbout, m_fxout, m_output;

extern "C" float lfo_eval(u32 ti, float warp, unsigned int shape);

typedef struct CalibResult {
  u16 pressure[8];
  s16 pos[8];
} CalibResult;

extern "C" CalibResult calibresults[18];
extern "C" int flash_readcalib(void);

int getheadphonevol(void);
uint32_t wang_hash(uint32_t seed);
int main(int argc, char **argv);
void Shutdown();
void EMSCRIPTEN_KEEPALIVE plinky_init(void);
void ApplyUF2File(const char *fname);
void plinky_frame(void);
void SleepMillis(int millis);
void EmuFrame();
void flip(void);
}