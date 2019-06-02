#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#define N 18
#define M 4

static const unsigned char f[N][M] =
{
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00},
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
