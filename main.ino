#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#define N 18
#define M 4

static const unsigned char f[N][M] =
{
    {0x07, 0xE6, 0x60, 0x5E},
    {0x40, 0x9E, 0x56, 0x2B},
    {0x00, 0x69, 0xA3, 0x7D},
    {0xA2, 0x3B, 0x78, 0x6B},
    {0x10, 0x9E, 0x5B, 0x2B},
    {0x70, 0xFA, 0x30, 0x7E},
    {0xF3, 0x24, 0x2A, 0x02},
    {0x80, 0x38, 0x1A, 0x7E},
    {0x70, 0x58, 0xAB, 0x2B},
    {0xD0, 0x1D, 0xA8, 0x2B},
    {0x07, 0x5C, 0xFC, 0x5D},
    {0x61, 0xC4, 0x41, 0x4F},
    {0x09, 0xE0, 0x34, 0x90},
    {0xB9, 0x88, 0xCE, 0x0A},
    {0x99, 0xCE, 0x29, 0x90},
    {0x52, 0xC6, 0x7A, 0x6B},
    {0x39, 0xBC, 0x45, 0x90},
    {0xD3, 0x04, 0x30, 0x02},
};

MFRC522 mfrc522(10, 9);
Servo mg90s;

void setup()
{
    SPI.begin();
    mfrc522.PCD_Init();

    // Serial.begin(9600);
    mg90s.attach(6);
    mg90s.write(0);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    return;
}

void loop()
{
    static int c;
    int i, j;

    if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
    {
        for(i = 0; i < N; i ++)
        {
            for(j = 0; j < M; j ++)
                if(mfrc522.uid.uidByte[j] != f[i][j])
                    break;
            if(j == M)
            {
                mg90s.write(80);
                analogWrite(5, 240);
                delay(250);
                analogWrite(5, 0);
                delay(2750);
                mg90s.write(0);
                c += 3;
                break;
            }
        }

        if(i == N)
        {
            analogWrite(5, 240);
            delay(100);
            analogWrite(5, 0);
            delay(100);
            analogWrite(5, 240);
            delay(100);
            analogWrite(5, 0);
            delay(100);
            analogWrite(5, 240);
            delay(100);
            analogWrite(5, 0);
            delay(3000);
            c += 3;
        }
    }
    else
    {
        delay(100);
        c ++;
    }

    // Serial.println(c);

    if(c >= 20)
    {
        c = 0;
        SPI.end();
        SPI.begin();
        mfrc522.PCD_Init();
    }

    return;
}
