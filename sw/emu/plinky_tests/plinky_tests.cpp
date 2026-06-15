#include "pch.h"
#include "CppUnitTest.h"
#include <thread>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern "C" {
#include "main.h"
#include "..\..\Core\Src\gfx.h"
}


#define _height 32
#define _width 128

namespace tests
{
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

            AssertScreen("initState.bmp");

            GetDisplay(disp1);

            clear();
            plinky_frame();

            putpixel(6, 0, 1);
            AssertScreen("initState.bmp");

            char *expectedLeds = 
                "x......x"
                "..x....."
                "....x..."
                "......x."
                ".x......"
                "...x...."
                ".....x.."
                "x......x"
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

            AssertScreen("soundC1.bmp");

            char* expectedLeds = 
                "x......x"
                "..x....."
                "....x..."
                "......x."
                ".x......"
                "...x...."
                "x....x.."
                "Xx.....x"
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

            AssertScreen("shiftB.bmp");
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

            ShiftInitWeirdnessFix(audioOut, audioin);

            pressButton(P_SHIFT_DOWN, audioOut, audioin);

            // First press weirdness...
            pressButton(P_DEGREE, audioOut, audioin);
            releaseButton(P_DEGREE, audioOut, audioin);
            pressButton(P_DEGREE, audioOut, audioin);
            releaseButton(P_DEGREE, audioOut, audioin);

            pressButton(P_DEGREE, audioOut, audioin);
            releaseButton(P_DEGREE, audioOut, audioin);

            pressButton(P_INPUT_X, audioOut, audioin);
            releaseButton(P_INPUT_X, audioOut, audioin);

            releaseButton(P_SHIFT_DOWN, audioOut, audioin);

            char *expectedLeds = 
                "...x...x"
                "..x....."
                "x...x..."
                "x.....x."
                "xx......"
                "x..x...."
                "x....x.."
                ".......x"
                ".x......";

            AssertLeds(expectedLeds);

            encval = 350;
            WaitMs(500, audioOut, audioin);

            AssertScreen("connectState.bmp");
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

        void WaitMs(int millis, u32 *audioOut, const u32 *audioin) {
            for (int i = 0; i < millis; i++) {
               millisEmu++;

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

        void pressButton(EParams param, u32 *audioOut, u32 *audioin, int baseOffset = 100, int waitMillis = 300) {
          settouch(param, 2000, audioin, audioOut, baseOffset, waitMillis);
        }

        void releaseButton(EParams param, u32 *audioOut, u32 *audioin, int baseOffset = 100, int waitMillis = 300) {
          settouch(param, 0, audioin, audioOut, baseOffset, waitMillis);
        }

        void settouch(EParams param, int pressure, u32 *audioOut, const u32 *audioin, int baseOffset = 100, int waitMillis = 300)
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
              if (v > 10    ) {
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

        void AssertScreen(char* name) {
          char *fname = (char *)malloc(strlen(name) + strlen("./../../plinky_tests/") + 1);
          strcpy(fname, "./../../plinky_tests/");
          strcpy(fname + strlen("./../../plinky_tests/"), name);

          char *screenBuffer = (char *)malloc(128*32);
          GetDisplay(screenBuffer);

          FILE *file;
          if ((file = fopen(fname, "r")) == NULL) {
            if (errno == ENOENT) 
            {
              write_bmp(fname, screenBuffer);
              free(screenBuffer);
              free(fname);
              Assert::Fail(L"Expectation file did not exists. Created.");
            }
          } else {
            fclose(file);
          }

          char *expectedBuffer = (char *)malloc(128 * 32);
          read_from_bmp(fname, expectedBuffer);

          for (int i = 0; i < 128 * 32; i++) 
          {
            if (screenBuffer[i] != expectedBuffer[i]) 
            {
              char* diffName = (char *)malloc(strlen(fname) + strlen(".diff.bmp") + 1);
              strcpy(diffName, fname);
              strcpy(diffName + strlen(fname), ".diff.bmp");

              write_bmp(diffName, screenBuffer);
              free(screenBuffer);
              free(expectedBuffer);
              free(diffName);
              free(fname);
              Assert::Fail(L"Mismatch between actual and expected. Actual file created as a *.diff.bmp.");
            }
          }

          free(screenBuffer);
          free(expectedBuffer);
          free(fname);
        }

        void write_bmp(char *filename, char* rgb) {
          int bitmap_size = _width * _height * 3;
          char *bitmap = (char *)malloc(bitmap_size * sizeof(char));

          for (int row = 0; row < _height; row++) {
            for (int col = 0; col < _width; col++) {

              for (int color = 0; color < 3; color++) {
                int index = (row * _width + col) * 3 + color;
                char val = rgb[(_height - row - 1) * _width + col];
                bitmap[index] = val == '_' ? 0 : 255;
              }
            }
          }

          char tag[] = {'B', 'M'};
          int header[] = {0, 0, 0x36, 0x28, _width, _height, 0x180001, 0, 0, 0x002e23, 0x002e23, 0, 0};
          header[0] = sizeof(tag) + sizeof(header) + bitmap_size;
          FILE *fp = fopen(filename, "w+");
          fwrite(&tag, sizeof(tag), 1, fp);
          fwrite(&header, sizeof(header), 1, fp);
          fwrite(bitmap, bitmap_size * sizeof(char), 1, fp);
          fclose(fp);
          free(bitmap);
        }

        void read_from_bmp(char *filename, char* rgb) {
          int bitmap_size = _width * _height * 3;
          char *bitmap = (char *)malloc(bitmap_size * sizeof(char));

          char tag[] = {'B', 'M'};
          int header[] = {0, 0, 0x36, 0x28, _width, _height, 0x180001, 0, 0, 0x002e23, 0x002e23, 0, 0};
          header[0] = sizeof(tag) + sizeof(header) + bitmap_size;
          FILE *fp = fopen(filename, "r+");
          fread(&tag, sizeof(tag), 1, fp);
          fread(&header, sizeof(header), 1, fp);
          fread(bitmap, bitmap_size * sizeof(char), 1, fp);
          fclose(fp);

          for (int row = 0; row < _height; row++) {
            for (int col = 0; col < _width; col++) {
              int index = (row * _width + col) * 3;
              char val = bitmap[index];
              rgb[(_height - row - 1) * _width + col] = (val == '\0' ? '_' : 'X');
            }
          }

          free(bitmap);
        }
	};
}
