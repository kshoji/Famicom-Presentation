const signed char sinCalc[256] = {
  /* This table rolls a lot of functions together for speed.
     Extracting phase and amplitude from the nybble packed form
     Sine calculation
     Exponential amplitude mapping
     Scaling to appropriate range
     
     ROUND(
       FLOOR(a/16,1)
       *SIN(
         2
         * PI()
         * IF(
           MOD(a,16),
           EXP(0.18*MOD(a,16)),
           0
         ) /16
       )*127
     ,0)
  */
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,2,2,3,3,4,5,6,7,8,10,12,14,17,20,24,
  0,4,4,5,6,7,9,11,13,15,18,22,26,31,37,45,
  0,5,6,7,8,10,12,14,17,20,24,28,34,41,49,58,
  0,5,6,7,9,10,12,15,18,21,26,31,37,44,53,63,
  0,5,6,7,8,10,12,14,17,20,24,28,34,41,49,58,
  0,4,4,5,6,7,9,11,13,15,18,22,26,31,37,45,
  0,2,2,3,3,4,5,6,7,8,10,12,14,17,20,24,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,-2,-2,-3,-3,-4,-5,-6,-7,-8,-10,-12,-14,-17,-20,-24,
  0,-4,-4,-5,-6,-7,-9,-11,-13,-15,-18,-22,-26,-31,-37,-45,
  0,-5,-6,-7,-8,-10,-12,-14,-17,-20,-24,-28,-34,-41,-49,-58,
  0,-5,-6,-7,-9,-10,-12,-15,-18,-21,-26,-31,-37,-44,-53,-63,
  0,-5,-6,-7,-8,-10,-12,-14,-17,-20,-24,-28,-34,-41,-49,-58,
  0,-4,-4,-5,-6,-7,-9,-11,-13,-15,-18,-22,-26,-31,-37,-45,
  0,-2,-2,-3,-3,-4,-5,-6,-7,-8,-10,-12,-14,-17,-20,-24
};

const signed char sqrCalc[256] ={
  0,1,2,2,2,3,3,4,5,5,6,8,9,11,13,16,
  0,1,2,2,2,3,3,4,5,5,6,8,9,11,13,16,
  0,1,2,2,2,3,3,4,5,5,6,8,9,11,13,16,
  0,1,2,2,2,3,3,4,5,5,6,8,9,11,13,16,
  0,1,2,2,2,3,3,4,5,5,6,8,9,11,13,16,
  0,1,2,2,2,3,3,4,5,5,6,8,9,11,13,16,
  0,1,2,2,2,3,3,4,5,5,6,8,9,11,13,16,
  0,1,2,2,2,3,3,4,5,5,6,8,9,11,13,16,
  0,-1,-2,-2,-2,-3,-3,-4,-5,-5,-6,-8,-9,-11,-13,-16,
  0,-1,-2,-2,-2,-3,-3,-4,-5,-5,-6,-8,-9,-11,-13,-16,
  0,-1,-2,-2,-2,-3,-3,-4,-5,-5,-6,-8,-9,-11,-13,-16,
  0,-1,-2,-2,-2,-3,-3,-4,-5,-5,-6,-8,-9,-11,-13,-16,
  0,-1,-2,-2,-2,-3,-3,-4,-5,-5,-6,-8,-9,-11,-13,-16,
  0,-1,-2,-2,-2,-3,-3,-4,-5,-5,-6,-8,-9,-11,-13,-16,
  0,-1,-2,-2,-2,-3,-3,-4,-5,-5,-6,-8,-9,-11,-13,-16,
  0,-1,-2,-2,-2,-3,-3,-4,-5,-5,-6,-8,-9,-11,-13,-16
};

enum {
  _SP,_DOT,_QM,_COM,_HYP,_IY,_IH,_EH,_AE,_AA,
  _AH,_AO,_UH,_AX,_IX,_ER,_UX,_OH,_RX,_LX,
  _WX,_YX,_WH,_R,_L,_W,_Y,_M,_N,_NX,
  _DX,_Q,_S,_SH,_F,_TH,__H,__X,_Z,_ZH,
  _V,_DH,_CHa,_CHb,_Ja,_Jb,_Jc,_Jd,_EY,_AY,
  _OY,_AW,_OW,_UW,_Ba,_Bb,_Bc,_Da,_Db,_Dc,
  _Ga,_Gb,_Gc,_GXa,_GXb,_GXc,_Pa,_Pb,_Pc,_Ta,
  _Tb,_Tc,_Ka,_Kb,_Kc,_KXa,_KXb,_KXc
};

enum {
  V_SP,
  V_DOT,
  V_QM,
  V_COM,
  V_HYP,
  V_A,
  V_I,
  V_U,
  V_E,
  V_O,
  V_K,
  V_S,
  V_T,
  V_N,
  V_H,
  V_M,
  V_Y,
  V_R,
  V_W,
  V_NN,
  V_G,
  V_Z,
  V_D,
  V_B,
};


#define FORMANT_SZ (7)

const unsigned char formantTable[] = {
   0x0, 0x0, 0x0,0x0,0x0,0x0,0x0,/*00 space*/ 0x13,0x43,0x5b,0x0,0x0,0x0,0x0,/*01 .*/
  0x13,0x43,0x5b,0x0,0x0,0x0,0x0,/*02 ?*/     0x13,0x43,0x5b,0x0,0x0,0x0,0x0,/*03 ,*/
  0x13,0x43,0x5b,0x0,0x0,0x0,0x0,/*04 -*/      0xa,0x54,0x6e,0xd,0xa,0x8,0x0,/*05 IY*/
   0xe,0x49,0x5d,0xd,0x8,0x7,0x0,/*06 IH*/    0x13,0x43,0x5b,0xe,0xd,0x8,0x0,/*07 EH*/
  0x18,0x3f,0x58,0xf,0xe,0x8,0x0,/*08 AE*/    0x1b,0x28,0x59,0xf,0xd,0x1,0x0,/*09 AA*/
  0x17,0x2c,0x57,0xf,0xc,0x1,0x0,/*10 AH*/    0x15,0x1f,0x58,0xf,0xc,0x0,0x0,/*11 AO*/
  0x10,0x25,0x52,0xf,0xb,0x1,0x0,/*12 UH*/    0x14,0x2c,0x57,0xe,0xb,0x0,0x0,/*13 AX*/
   0xe,0x49,0x5d,0xd,0xb,0x7,0x0,/*14 IX*/    0x12,0x31,0x3e,0xc,0xb,0x5,0x0,/*15 ER*/
   0xe,0x24,0x52,0xf,0xc,0x1,0x0,/*16 UX*/    0x12,0x1e,0x58,0xf,0xc,0x0,0x0,/*17 OH*/
  0x12,0x33,0x3e,0xd,0xc,0x6,0x0,/*18 RX*/    0x10,0x25,0x6e,0xd,0x8,0x1,0x0,/*19 LX*/
   0xd,0x1d,0x50,0xd,0x8,0x0,0x0,/*20 WX*/     0xf,0x45,0x5d,0xe,0xc,0x7,0x0,/*21 YX*/
   0xb,0x18,0x5a,0xd,0x8,0x0,0x0,/*22 WH*/    0x12,0x32,0x3c,0xc,0xa,0x5,0x0,/*23 R*/
   0xe,0x1e,0x6e,0xd,0x8,0x1,0x0,/*24 L*/      0xb,0x18,0x5a,0xd,0x8,0x0,0x0,/*25 W*/
   0x9,0x53,0x6e,0xd,0xa,0x8,0x0,/*26 Y*/      0x6,0x2e,0x51,0xc,0x3,0x0,0x0,/*27 M*/
   0x6,0x36,0x79,0x9,0x9,0x0,0x0,/*28 N*/      0x6,0x56,0x65,0x9,0x6,0x3,0x0,/*29 NX*/
   0x6,0x36,0x79,0x0,0x0,0x0,0x0,/*30 DX*/    0x11,0x43,0x5b,0x0,0x0,0x0,0x0,/*31 Q*/
   0x6,0x49,0x63,0x7,0xa,0xd,0xf,/*32 S*/      0x6,0x4f,0x6a,0x0,0x0,0x0,0x0,/*33 SH*/
   0x6,0x1a,0x51,0x3,0x3,0x3,0xf,/*34 F*/      0x6,0x42,0x79,0x0,0x0,0x0,0x0,/*35 TH*/
   0xe,0x49,0x5d,0x0,0x0,0x0,0x0,/*36 /H*/    0x10,0x25,0x52,0x0,0x0,0x0,0x0,/*37 /X*/
   0x9,0x33,0x5d,0xf,0x3,0x0,0x3,/*38 Z*/      0xa,0x42,0x67,0xb,0x5,0x1,0x0,/*39 ZH*/
   0x8,0x28,0x4c,0xb,0x3,0x0,0x0,/*40 V*/      0xa,0x2f,0x5d,0xb,0x4,0x0,0x0,/*41 DH*/
   0x6,0x4f,0x65,0x0,0x0,0x0,0x0,/*42 CHa*/    0x6,0x4f,0x65,0x0,0x0,0x0,0x0,/*43 CHb*/
   0x6,0x42,0x79,0x1,0x0,0x0,0x0,/*44 Ja*/     0x5,0x42,0x79,0x1,0x0,0x0,0x0,/*45 Jb*/
   0x6,0x6e,0x79,0x0,0xa,0xe,0x0,/*46 Jc*/     0x0, 0x0, 0x0,0x2,0x2,0x1,0x0,/*47 Jd*/
  0x13,0x48,0x5a,0xe,0xe,0x9,0x0,/*48 EY*/    0x1b,0x27,0x58,0xf,0xd,0x1,0x0,/*49 AY*/
  0x15,0x1f,0x58,0xf,0xc,0x0,0x0,/*50 OY*/    0x1b,0x2b,0x58,0xf,0xd,0x1,0x0,/*51 AW*/
  0x12,0x1e,0x58,0xf,0xc,0x0,0x0,/*52 OW*/     0xd,0x22,0x52,0xd,0x8,0x0,0x0,/*53 UW*/
   0x6,0x1a,0x51,0x2,0x0,0x0,0x0,/*54 Ba*/     0x6,0x1a,0x51,0x4,0x1,0x0,0xf,/*55 Bb*/
   0x6,0x1a,0x51,0x0,0x0,0x0,0x0,/*56 Bc*/     0x6,0x42,0x79,0x2,0x0,0x0,0x0,/*57 Da*/
   0x6,0x42,0x79,0x4,0x1,0x0,0xf,/*58 Db*/     0x6,0x42,0x79,0x0,0x0,0x0,0x0,/*59 Dc*/
   0x6,0x6e,0x70,0x1,0x0,0x0,0x0,/*60 Ga*/     0x6,0x6e,0x6e,0x4,0x1,0x0,0xf,/*61 Gb*/
   0x6,0x6e,0x6e,0x0,0x0,0x0,0x0,/*62 Gc*/     0x6,0x54,0x5e,0x1,0x0,0x0,0x0,/*63 GXa*/
   0x6,0x54,0x5e,0x4,0x1,0x0,0xf,/*64 GXb*/    0x6,0x54,0x5e,0x0,0x0,0x0,0x0,/*65 GXc*/
   0x6,0x1a,0x51,0x0,0x0,0x0,0x0,/*66 Pa*/     0x6,0x1a,0x51,0x0,0x0,0x0,0x0,/*67 Pb*/
   0x6,0x1a,0x51,0x0,0x0,0x0,0x0,/*68 Pc*/     0x6,0x42,0x79,0x0,0x0,0x0,0x0,/*69 Ta*/
   0x6,0x42,0x79,0x0,0x0,0x0,0x0,/*70 Tb*/     0x6,0x42,0x79,0x0,0x0,0x0,0x0,/*71 Tc*/
   0x6,0x6d,0x65,0x0,0x0,0x0,0x0,/*72 Ka*/     0xa,0x56,0x65,0xc,0xa,0x7,0x0,/*73 Kb*/
   0xa,0x6d,0x70,0x0,0x0,0x0,0x0,/*74 Kc*/     0x6,0x54,0x5e,0x0,0x0,0x0,0x0,/*75 KXa*/
   0x6,0x54,0x5e,0x0,0xa,0x5,0x0,/*76 KXb*/    0x6,0x54,0x5e,0x0,0x0,0x0,0x0 /*77 KXc*/
};

// TODO


unsigned short pitchPhase, form1Phase,form2Phase,form3Phase;
unsigned short pitchPhaseInc,form1PhaseInc,form2PhaseInc,form3PhaseInc;
unsigned char form1Amp,form2Amp,form3Amp;
unsigned char noiseMod=10;
unsigned char waveCalc() {
  unsigned char value;
  static char noise;
  int phaseNoise = noise * noiseMod;
  noise += noise<<2; noise++;  // noise' = 5*noise+1
  
  form1Phase += form1PhaseInc;
  value = sinCalc[(((form1Phase>>8) & 0xf0) | form1Amp)];
  form2Phase += form2PhaseInc;
  value += sinCalc[(((form2Phase>>8) & 0xf0) | form2Amp)];
  form3Phase += form3PhaseInc;
  value += sqrCalc[(((form3Phase>>8) & 0xf0) | form3Amp)];

  value = (value * (0xff^(pitchPhase>>8)))>>8;
  pitchPhase += pitchPhaseInc;
  if ((pitchPhase+phaseNoise) < pitchPhaseInc) {
    form1Phase = 0;
    form2Phase = 0;
    form3Phase = 0;
  }
  return value + 0x80;
}

#define formantScale (54)
void speech(unsigned char startFormant, unsigned char staticFrames, unsigned char tweenFrames, unsigned int startPitch, unsigned int nextPitch, unsigned char nextFormant) {
  //while(staticFrames) 
  {
    int index;
    unsigned short startForm1PhaseInc,startForm2PhaseInc,startForm3PhaseInc;
    unsigned char startForm1Amp,startForm2Amp,startForm3Amp;
    unsigned char startMod;
    unsigned char *formantPos;

    // Read next framelist item
    //if (!staticFrames) break; // End of phrase

    pitchPhaseInc = startPitch;
    formantPos = (unsigned char *)(formantTable + startFormant * FORMANT_SZ);
    form1PhaseInc = startForm1PhaseInc = *(formantPos++)*formantScale;
    form2PhaseInc = startForm2PhaseInc = *(formantPos++)*formantScale;
    form3PhaseInc = startForm3PhaseInc = *(formantPos++)*formantScale;
    form1Amp = startForm1Amp = *(formantPos++);
    form2Amp = startForm2Amp = *(formantPos++);
    form3Amp = startForm3Amp = *(formantPos++);
    noiseMod = startMod = *(formantPos++);

    for (;staticFrames--;) ;//delay(1); // 15 msec: 1 frame in NTSC
    if (tweenFrames) {
      unsigned char* formantPos;
      short deltaForm1PhaseInc,deltaForm2PhaseInc,deltaForm3PhaseInc;
      char deltaForm1Amp,deltaForm2Amp,deltaForm3Amp;
      char deltaMod;
      unsigned char nextMod;
      short deltaPitch;
      tweenFrames--;
      formantPos = (unsigned char *)(formantTable + nextFormant * FORMANT_SZ);
      deltaForm1PhaseInc = *(formantPos++)*formantScale - startForm1PhaseInc;
      deltaForm2PhaseInc = *(formantPos++)*formantScale - startForm2PhaseInc;
      deltaForm3PhaseInc = *(formantPos++)*formantScale - startForm3PhaseInc;
      deltaForm1Amp = *(formantPos++) - startForm1Amp;
      deltaForm2Amp = *(formantPos++) - startForm2Amp;
      deltaForm3Amp = *(formantPos++) - startForm3Amp;
      deltaMod = *(formantPos++) - startMod;
      deltaPitch = nextPitch - startPitch;
      deltaMod = nextMod - startMod;
      for (index=1; index<=tweenFrames; index++) {
        form1PhaseInc = startForm1PhaseInc + (index*deltaForm1PhaseInc)/tweenFrames;
        form2PhaseInc = startForm2PhaseInc + (index*deltaForm2PhaseInc)/tweenFrames;
        form3PhaseInc = startForm3PhaseInc + (index*deltaForm3PhaseInc)/tweenFrames;
        form1Amp = startForm1Amp + (index*deltaForm1Amp)/tweenFrames;
        form2Amp = startForm2Amp + (index*deltaForm2Amp)/tweenFrames;
        form3Amp = startForm3Amp + (index*deltaForm3Amp)/tweenFrames;
        pitchPhaseInc = startPitch + (index*deltaPitch)/tweenFrames;
        noiseMod = startMod + (index*deltaMod)/tweenFrames;
        //delay(1);

        *((unsigned char*)0x4011) = waveCalc();
      }
    }
  }
}

void speech_japanese(unsigned char formant, unsigned char staticFrames, unsigned char tweenFrames, unsigned int startPitch, unsigned int nextPitch, unsigned char nextFormant) {
  *((unsigned char*)0x4010) = 0x88;
  switch(formant) {
    case V_SP:
    case V_DOT:
    case V_QM:
    case V_COM:
    case V_HYP:
    break;
    case V_A:
    speech(_AA, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_I:
    speech(_IH, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_U:
    speech(_UH, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_E:
    speech(_EH, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_O:
    speech(_OH, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_K:
    speech(_Ka, staticFrames, tweenFrames, startPitch, nextPitch, _Kb);
    speech(_Kb, staticFrames, tweenFrames, startPitch, nextPitch, _Kc);
    speech(_Kc, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_S:
    speech(_S, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_T:
    speech(_Ta, staticFrames, tweenFrames, startPitch, nextPitch, _Tb);
    speech(_Tb, staticFrames, tweenFrames, startPitch, nextPitch, _Tc);
    speech(_Tc, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_N:
    speech(_N, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_H:
    speech(__H, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_M:
    speech(_M, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_Y:
    speech(_Y, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_R:
    speech(_R, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_W:
    speech(_W, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_NN:
    speech(_NX, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_G:
    speech(_Ga, staticFrames, tweenFrames, startPitch, nextPitch, _Gb);
    speech(_Gb, staticFrames, tweenFrames, startPitch, nextPitch, _Gc);
    speech(_Gc, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_Z:
    speech(_Z, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_D:
    speech(_Da, staticFrames, tweenFrames, startPitch, nextPitch, _Db);
    speech(_Db, staticFrames, tweenFrames, startPitch, nextPitch, _Dc);
    speech(_Dc, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
    case V_B:
    speech(_Ba, staticFrames, tweenFrames, startPitch, nextPitch, _Bb);
    speech(_Bb, staticFrames, tweenFrames, startPitch, nextPitch, _Bc);
    speech(_Bc, staticFrames, tweenFrames, startPitch, nextPitch, nextFormant);
    break;
  }
}

