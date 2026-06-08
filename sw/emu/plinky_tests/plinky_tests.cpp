#include "pch.h"
#include "CppUnitTest.h"
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern "C" {
#include "main.h"
#include "..\..\Core\Src\gfx.h"
}

namespace tests
{
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

	TEST_CLASS(tests)
	{
	public:
        TEST_METHOD(StartupState)
		{	
            u32 audioin[BLOCK_SAMPLES];
            u32 audioOut[BLOCK_SAMPLES];
            char disp1[128 * 32];

            // Init
            enable_emu_audio = false;
            plinky_init();

            // Fill the initial scope and lfo history buffers and run the init animations
            WaitMs(5000, audioOut, audioin);

            ShiftInitWeirdnessFix(audioOut, audioin);

            // Assert stable screen
            GetDisplay(disp1);
            uitick(audioOut, audioin, 0);
            plinky_frame();
            plinky_frame();
            plinky_frame();
            plinky_frame();
            AssertDisplay(disp1, true);

            char* initialized = 
"______XXXXXXX_________XX________________________________________________________________________________________________________"
"_____XXXXXXXX________XXX________________________________________________________________________________________________________"
"____XX_XX__XX________XXX________________________________________________________________________________________________________"
"___XX__XX__XX_______XXXX________________________________________________________________________________________XXXXXXXXXXXXXXXX"
"__XX___XX__XX_______XXXX________________________________________________________________________________________________________"
"__XXXXXXX__XX_________XX________________________________________________________________________________________________________"
"__XXXXXXX__XX_________XX________________________________________________________________________________________________________"
"__XX_______XX_________XX________________________________________________________________________________________________________"
"__XX_______XX_________XX________________________________________________________________________________________________________"
"__XX_______XX_________XX________________________________________________________________________________________________________"
"__XX_______XX_________XX________________________________________________________________________________________________________"
"__XX_______XX_________XX________________________________________________________________________________________XXXXXXXXXXXXXXXX"
"__XX_______XX_________XX________________________________________________________________________________________________________"
"__XXXXXXXXXXX_________XX________________________________________________________________________________________________________"
"__XXXXXXXXXXX_________XX________________________________________________________________________________________________________"
"________________________________________________________________________________________________________________________________"
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
"_____________________XXX________________________________________________________________________________________________________"
"_____________________XXX________________________________________________________________________________________________________"
"_XX___XX___XX_______XXXX________________________________________________________________________________________XXXXXXXXXXXXXXXX"
"X__X_X__X_X__X______XXXX________________________________________________________________________________________________________"
"X__X_X__X_X__X________XX________________________________________________________________________________________________________"
"_XX___XX___XX_________XX________________________________________________________________________________________________________"
"______________________XX________________________________________________________________________________________________________"
"_XX___XX___XX_________XX________________________________________________________________________________________________________"
"XXXX_X__X_X__X________XX________________________________________________________________________________________________________"
"XXXX_X__X_X__X________XX________________________________________________________________________________________________________"
"_XX___XX___XX_________XX________________________________________________________________________________________XXXXXXXXXXXXXXXX"
"______________________XX________________________________________________________________________________________________________"
"______________________XX________________________________________________________________________________________________________"
"______________________XX________________________________________________________________________________________________________"
"___________________________________________XXX_____XXX______XXX______XXX______XXX_____XXX______XXX______XXX_____________________";

            AssertDisplay(initialized, true);

            GetDisplay(disp1);

            putpixel(5, 0, 1);
            AssertDisplay(disp1, false);

            clear();
            plinky_frame();

            putpixel(6, 0, 1);
            AssertDisplay(disp1, true);

            char *expectedLeds = 
                "?......."
                "........"
                "........"
                "........"
                "........"
                "........"
                "........"
                "........"
                "........";

            AssertLeds(expectedLeds);
		}

        TEST_METHOD(SimpleInputs)
		{	
            u32 audioin[BLOCK_SAMPLES];
            u32 audioOut[BLOCK_SAMPLES];
            char disp1[128 * 32];

            // Init
            enable_emu_audio = false;
            plinky_init();

            // Fill the initial scope and lfo history buffers and run the init animations
            WaitMs(5000, audioOut, audioin);

            ShiftInitWeirdnessFix(audioOut, audioin);

            settouch(0, 7, 2047);
            WaitMs(300, audioOut, audioin);

            char* initialized = 
"___XXXX__________XX_____________________________________________________________________________________________X_______XXXXXXX_"
"_XXXXXXXX_______XXX_____________________________________________________________________________________________X_____________XX"
"_XX____XX_______XXX___________________________________________________________________________________________________________XX"
"XX_____________XXXX____________________________________________________________________________________________XXXXXXXXXXXXXXXXX"
"XX_____________XXXX____________________________________________________________________________________________X______________XX"
"XX_______________XX___________________________________________________________________________________________________________XX"
"XX_______________XX___________________________________________________________________________________________________________XX"
"XX_______________XX___________________________________________________________________________________________________________XX"
"XX_______________XX___________________________________________________________________________________________________________XX"
"XX_______________XX___________________________________________________________________________________________________________XX"
"XX_______________XX____________________________________________________________________________________________X______________XX"
"XX_______________XX_____________________________________________________________________________________________XXXXXXXXXXXXXXXX"
"_XX____XX________XX____________________________________________________________________________________________X______________XX"
"_XXXXXXXX________XX___________________________________________________________________________________________________________XX"
"___XXXX__________XX___________________________________________________________________________________________________________XX"
"______________________________________________________________________________________________________________________________XX"
"______________________XX______________________________________________________________________________________________________XX"
"_____________________XXX______________________________________________________________________________________________________XX"
"XX___________________XXX______________________________________________________________________________________________________XX"
"_XXXXXXXXX_XX_______XXXX________________________________________________________________________________________XXXXXXXXXXXXXXXX"
"X__X_X__X_XXXXXXXX__XXXX___________________XXX________________________________________________________________________________XX"
"X__X_X__X_X__X____XXXXXXXXX_________________X_________________________________________________________________X_______________XX"
"_XX___XX___XX_________XX__XXXXXXXXXX_______X_X________________________________________________________________________________XX"
"______________________XX____________XXXXXXXXXX________________________________________________________________X_______________XX"
"_XX___XX___XX_________XX___________________X_XXXXXXXXXXXX_______________________________________________________________________"
"XXXX_X__X_X__X________XX____________________X____________XXXXXXXXXXXX___________________________________________________________"
"XXXX_X__X_X__X________XX___________________X_X_______________________XXXXXXXXXXXXXX_____________________________________________"
"_XX___XX___XX_________XX____________________X_____________________________________XXXXXXXXXXXXXXXX______________XXXXXXXXXXXXXXXX"
"______________________XX___________________X_X___________________________________________________XXXXXXXXXXXXXX_________________"
"______________________XX____________________X___________________________________________________________________________________"
"______________________XX___________________X_X__________________________________________________________________________________"
"___________________________________________________XXX______XXX______XXX______XXX_____XXX______XXX______XXX_____________________";

             AssertDisplay(initialized, true);

            char* expectedLeds = 
                "x......."
                "........"
                "........"
                "........"
                "........"
                "........"
                "........"
                "X......."
                "........";

            AssertLeds(expectedLeds);

            settouch(0, 7, 0);

            settouch(8, 1, 2000);
            WaitMs(1000, audioOut, audioin);

            expectedLeds = 
                ".......?"
                ".......?"
                ".......?"
                ".......?"
                ".......?"
                ".......?"
                ".......?"
                ".......?"
                ".X......";

            AssertLeds(expectedLeds);

            initialized = 
"____________________XXXXXXX_____________________________________________________________________________________________________"
"__X____X____X_______XXXXXXXX____________________________________________________________________________________________________"
"__X____X____X_______XX_____XX___________________________________________________________________________________________________"
"XXXXX__X____X_______XX_____XX___________________________________________________________________________________________________"
"XXXXX__X____X_______XX___XXX____________________________________________________________________________________________________"
"__X____X____X_______XXXXXXX_______XXXX_XX____XXXX_______XXXX____________________________________________________________________"
"__X____X__XXXXX_____XXXXXXXXX___XXXXXXXXX__XXXXXXXX___XXXXXXXX__________________________________________________________________"
"__X____X__XXXXX_____XX_____XX___XX____XXX_XXXX___XX___XX____XX__________________________________________________________________"
"__X____X____X_______XX______XX_XX______XX___XXXX_____XX______XX_________________________________________________________________"
"__X____X____X_______XX______XX_XX______XX_____XXXX___XXXXXXXXXX_________________________________________________________________"
"__X__XXXXX__X_______XX______XX_XX______XX_______XXXX_XXXXXXXXXX_________________________________________________________________"
"__X__XXXXX__X_______XX______XX_XX______XX_________XX_XX_________________________________________________________________________"
"__X____X____X_______XX_____XX___XX____XXX__XX____XX___XX____XX__________________________________________________________________"
"__X____X____X_______XXXXXXXXX___XXXXXXXXX__XXXXXXXX___XXXXXXXX__________________________________________________________________"
"____________________XXXXXXX_______XXXX_XX____XXXX_______XXXX____________________________________________________________________"
"________________________________________________________________________________________________________________________________"
"__________________X_____________________________________________________________________________________________________________"
"__________________X_____________________________________________________________________________________________________________"
"__________________X_____________________________________________________________________________________________________________"
"__________________X_____________________________________________________________________________________________________________"
"__XXXX_____XXXX___X___XXXX_____XXXX__X___________X_XXXX_____XXXX_X_X_XXXX____XXXX_X_XXX__XX____XXXX___X_________XXXX___X_XXXX___"
"_X____X___X____X__X__X____X___X____X_X___________XX____X___X____XX_XX____X__X____XX_X__XX__X__X____X__X________X____X__XX____X__"
"XX_______X______X_X_X______X_X_______X___________X______X_X______X_X_______X______X_X___X__X_X______X_X_______X______X_X________"
"__XX_____X______X_X_X______X_X_______XXXXX_______X______X_X______X_X_______X______X_X___X__X_X______X_XXXXX___X______X_X________"
"____XX___XXXXXXXX_X_XXXXXXXX_X_______X___________X______X_X______X_X_______X______X_X___X__X_XXXXXXXX_X_______XXXXXXXX_X________"
"______XX_X________X_X________X_______X___________X______X_X______X_X_______X______X_X___X__X_X________X_______X________X________"
"_X____X___X____X__X__X____X___X____X__X____X_____XX____X___X____XX_X________X____XX_X______X__X____X___X____X__X____X__X________"
"__XXXX_____XXXX___X___XXXX_____XXXX____XXXX______X_XXXX_____XXXX_X_X_________XXXX_X_X______X___XXXX_____XXXX____XXXX___X________"
"_________________________________________________X______________________________________________________________________________"
"_________________________________________________X______________________________________________________________________________"
"_________________________________________________X______________________________________________________________________________"
"_________________________________________________X______________________________________________________________________________";

            AssertDisplay(initialized, true);
		}

        TEST_METHOD(SimpleParams)
		{	
            u32 audioin[BLOCK_SAMPLES];
            u32 audioOut[BLOCK_SAMPLES];
            char disp1[128 * 32];

            // Init
            enable_emu_audio = false;
            plinky_init();

            // Fill the initial scope and lfo history buffers and run the init animations
            WaitMs(5000, audioOut, audioin);

            ShiftInitWeirdnessFix(audioOut, audioin);

            emu_setadc(0.5f, 0.5f, 12, 2, 3, -4, -1, 3, 0, 0, 0); // nop
            WaitMs(300, audioOut, audioin);

            encval += 5; // nop
            WaitMs(100, audioOut, audioin);

            encval -= 5; // nop
            WaitMs(100, audioOut, audioin);

            encbtn = 1; // nop
            WaitMs(100, audioOut, audioin);

            encbtn = 0; // nop
            WaitMs(100, audioOut, audioin);

            settouch(3, 4, 2047);
            WaitMs(500, audioOut, audioin);

            Assert::AreEqual(-48.1648f, gainhistoryrms [ghi], 0.001f);

            Assert::AreEqual(0.385687f, emucvout [2][emucvouthist], 0.001f); // pressure

            Assert::AreEqual(0.312069f, m_dry, 0.001f);
            Assert::AreEqual(1.25f, m_compressor, 0.001f);
            Assert::AreEqual(0.156212f, m_reverbin, 0.001f);
            Assert::AreEqual(0.0487366f, m_reverbout, 0.001f);
            Assert::AreEqual(0.0487366f, m_fxout, 0.001f);
            Assert::AreEqual(0.335205f, m_output, 0.001f);
		}

        TEST_METHOD(CvInput)
		{	
            u32 audioin[BLOCK_SAMPLES];
            u32 audioOut[BLOCK_SAMPLES];
            char disp1[128 * 32];

            // Init
            enable_emu_audio = false;
            plinky_init();

            // Fill the initial scope and lfo history buffers and run the init animations
            WaitMs(5000, audioOut, audioin);

            EditModeInitWeirdnessFix(audioOut, audioin);

            pressButton(P_SHIFT_DOWN, audioOut, audioin);

            pressButton(P_DEGREE, audioOut, audioin);
            releaseButton(P_DEGREE, audioOut, audioin);

            char *expectedLeds = 
                "...X...?"
                "........"
                "........"
                "........"
                "........"
                "........"
                "........"
                "........"
                ".X......";

            AssertLeds(expectedLeds);
		}

        void ShiftInitWeirdnessFix(u32 *audioOut, const u32 *audioin)
        {
            settouch(8, 1, 1500, 150);
            WaitMs(100, audioOut, audioin);

            settouch(8, 1, 2000, 120);
            WaitMs(100, audioOut, audioin);

            settouch(8, 1, 0, 110);
            WaitMs(1000, audioOut, audioin);
        }

        void EditModeInitWeirdnessFix(u32 *audioOut, const u32 *audioin) {
          settouch(8, 1, 1500, 150);
          WaitMs(100, audioOut, audioin);

          settouch(8, 1, 2000, 120);
          WaitMs(100, audioOut, audioin);

          settouch(8, 1, 0, 110);
          WaitMs(1000, audioOut, audioin);

          settouch(8, 1, 1500, 150);
          WaitMs(100, audioOut, audioin);

          settouch(1, 1, 2000, 120);
          WaitMs(100, audioOut, audioin);

          settouch(1, 1, 2000, 110);
          WaitMs(100, audioOut, audioin);

          settouch(1, 1, 0, 110);
          WaitMs(100, audioOut, audioin);

          settouch(8, 1, 0, 110);
          WaitMs(1000, audioOut, audioin);
        }

        void WaitMs(int millis, u32 *audioOut, const u32 *audioin) {
            for (int i = 0; i < millis; i++) {
              // 500hz - no half-frames
              if (i % 2 == 0) {
                uitick(audioOut, audioin, 0);
              }

              // ~21hz
              if (i % 48 == 0) {
                plinky_frame();
              }
            }
        }

        void pressButton(EParams param, u32 *audioOut, u32 *audioin, int baseOffset = 100, int waitMillis = 1000) {
          settouch(param, 1500, audioin, audioOut, baseOffset + 50, 200);
          settouch(param, 2000, audioin, audioOut, baseOffset, waitMillis);
        }

        void releaseButton(EParams param, u32 *audioOut, u32 *audioin, int baseOffset = 100, int waitMillis = 1000) {
          settouch(param, 0, audioin, audioOut, baseOffset, waitMillis);
        }

        void settouch(EParams param, int pressure, u32 *audioOut, const u32 *audioin, int baseOffset = 100, int waitMillis = 1000)
        {
            // TODO: first col, last col, shift row
          int columnIdx = 1 + (param % 6);
          int rowIdx = param / 12;

          if (param >= 96 && param < 104)
          {
            columnIdx = 0;
            rowIdx = param - 96;
          } 
          else if (param >= 104 && param < 112) 
          {
            columnIdx = 7;
            rowIdx = param - 104;
          } 
          else if (param >= 112) {
            columnIdx = 8;
            rowIdx = param - 112;
          } 

          emutouch[columnIdx][1] = baseOffset + rowIdx * 255, emutouch[columnIdx][0] = pressure;

          WaitMs(waitMillis, audioOut, audioin);
        }

        /// <summary>
        ///    01234567
        ///    ¨¨¨¨¨¨¨¨
        ///    0|||||||
        ///    ||||||||
        ///    ||||||||
        ///    ||||||||
        /// 8: 0-------
        /// </summary>
        /// <param name="idx">0-8, 0-7 are the vertical rows 8 is the bottom horizontal row.</param>
        /// <param name="pos">0-7, 0 s top/left</param>
        /// <param name="pressure">0-2047</param>
        void settouch(int idx, int pos, int pressure, int baseOffset = 100) {
          if (idx >= 0 && idx < 9)
            emutouch[idx][1] = baseOffset + pos * 255, emutouch[idx][0] = pressure;
        }

        void GetDisplay(char *buf) { 
            for (int yi = 0; yi < 32; yi++) {
              for (int xi = 0; xi < 128; xi++) {
                char val = (emupixels[yi * 128 + xi] == 0xffffffff) ? 'X' : '_';
                buf[(yi * 128) + xi] = val;
              }
            }
        }

        void AssertLeds(char* expected) {
          int firstDiffLine = -1;
          char buf[9 * 8];

          char bufPretty[9 * 9];
          char expectedPretty[9 * 9];

          for (int yi = 0; yi < 9; yi++) {
            for (int xi = 0; xi < 8; xi++) {
              u8 v = yi == 8 ? emuleds[yi][xi] : emuleds[xi][yi];
              char val = '.';
              if (v > 50) {
                val = 'x';
              }
              if (v > 150) {
                val = 'X';
              }

              buf[(yi * 8) + xi] = val;
              bufPretty[(yi * 9) + xi] = val;
              char exp = expected[yi * 8 + xi];
              expectedPretty[(yi * 9) + xi] = exp;

              if (exp != '?' && val != exp && firstDiffLine == -1) {
                firstDiffLine = yi;
              }
            }

            bufPretty[(yi * 9) + 8] = '\n';
            expectedPretty[(yi * 9) + 8] = '\n';
          }

          const size_t cSize = strlen(bufPretty) + 1;
          wchar_t *wc = new wchar_t[cSize];
          size_t tmp = 0;
          mbstowcs_s(&tmp, wc, cSize, bufPretty, cSize);

          Assert::IsFalse(firstDiffLine > -1, wc);
        }

        void AssertDisplay(char* expected, bool expectMatch) {
            flip();

            int firstDiffLine = -1;
            char buf[128 * 32];

            char diffPretty[129 * 32];
            char bufPretty[129 * 32];
            char expectedPretty[129 * 32];

            for (int yi = 0; yi < 32; yi++) {
              for (int xi = 0; xi < 128; xi++) {
                char val = (emupixels[yi * 128 + xi] == 0xffffffff) ? 'X' : '_';
                buf[(yi * 128) + xi] = val;
                bufPretty[(yi * 129) + xi] = val;

                char exp = expected[yi * 128 + xi];
                expectedPretty[(yi * 129) + xi] = exp;

                diffPretty[(yi * 129) + xi] = val;
                if (exp != '?' && buf[(yi * 128) + xi] != exp) 
                {
                  diffPretty[(yi * 129) + xi] = val == 'X' ? '+' : '-';
                  if (firstDiffLine == -1) {
                    firstDiffLine = yi;
                  }
                }
              }

              diffPretty[(yi * 129) + 128] = '\n';
              bufPretty[(yi * 129) + 128] = '\n';
              expectedPretty[(yi * 129) + 128] = '\n';
            }

            const size_t cSize = strlen(diffPretty) + 1 - 129 * (firstDiffLine > 1 ? firstDiffLine - 1 : 0);
            wchar_t *wc = new wchar_t[cSize];
            size_t tmp = 0;
            mbstowcs_s(&tmp, wc, cSize, diffPretty + 129 * (firstDiffLine > 1 ? firstDiffLine - 1 : 0), cSize);
            wc[0] = '\n';

            if (expectMatch) 
            {
              Assert::IsFalse(firstDiffLine > -1, wc);
            } else 
            {
              Assert::IsTrue(firstDiffLine > -1, wc);
            }
        }
	};
}
