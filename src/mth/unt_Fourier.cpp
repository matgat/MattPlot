#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include <cmath>  // fastmath
//---------------------------------------------------------------------------
#include "unt_Fourier.h"
//---------------------------------------------------------------------------



////////////////////////////////// cls_fft //////////////////////////////////

/*    The Discrete Fourier Transform of the complex sequence X of
      size N is the complex seqeunce Y (N long) given by:

                   __ N-1            -j (h·k · 2pi/N)
      Yk = F(Xk) = >          Xh ·  e
                   ¯¯ h,k=0

    The inverse transformation F`¹ is given by:
    F`¹ (x[k]) = F(x[N-1-k]) / N
    F`¹ (x) = F*( x* ) / N          , * is the cplx conj
    F`¹ (x) = swap{F(swap{x}} / N   , swap{} swaps Re with Im
    The last one is the best way

    When X is real:
    Y[k] = Y*[N-k-1]  (the spectrum |Y| is even)
    So it is sufficient to know Y in 0÷(N/2)
    The "DC" element Y0 is purely real.
    For even N the "Nyquist" element at N/2 is also real,
    so there are exactly N non-redundant real numbers in the
    first half + Nyquist element of the complex output Y
*/


// . . . Settings
// Prime numbers
// 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
// 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
// 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,...
// 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997
// The largest prime factor of N must be less than or equal to this:
static const int maxPrimeFactor = 37; // 71
static const int maxPrimeFactorDiv2 = (maxPrimeFactor+1)/2;
static const int maxFactorCount = 20;

// . . . Optimization constants
static const cls_fft::TReal  c3_1 = -1.5000000000000E+00;  //  c3_1 = cos(2*pi/3)-1;
static const cls_fft::TReal  c3_2 =  8.6602540378444E-01;  //  c3_2 = sin(2*pi/3);
static const cls_fft::TReal  u5   =  1.2566370614359E+00;  //  u5   = 2*pi/5;
static const cls_fft::TReal  c5_1 = -1.2500000000000E+00;  //  c5_1 = (cos(u5)+cos(2*u5))/2-1;
static const cls_fft::TReal  c5_2 =  5.5901699437495E-01;  //  c5_2 = (cos(u5)-cos(2*u5))/2;
static const cls_fft::TReal  c5_3 = -9.5105651629515E-01;  //  c5_3 = -sin(u5);
static const cls_fft::TReal  c5_4 = -1.5388417685876E+00;  //  c5_4 = -(sin(u5)+sin(2*u5));
static const cls_fft::TReal  c5_5 =  3.6327126400268E-01;  //  c5_5 = (sin(u5)-sin(2*u5));
static const cls_fft::TReal  c8   =  7.0710678118655E-01;  //  c8 = 1/sqrt(2);

// . . . Static attributes

// . . . Other bags
static cls_fft::TReal trigRe[maxPrimeFactor],
                      trigIm[maxPrimeFactor];

static cls_fft::TReal zRe[maxPrimeFactor],
                      zIm[maxPrimeFactor];





//------------------------------ Public methods -----------------------------

//---------------------------------------------------------------------------
//  FFT transformation
void cls_fft::fft(int N, TReal xRe[], TReal xIm[], TReal yRe[], TReal yIm[])
{
    int sofarRadix[maxFactorCount],
        actualRadix[maxFactorCount],
        remainRadix[maxFactorCount];
    int nFactor;

    transTableSetup(sofarRadix, actualRadix, remainRadix, &nFactor, &N);
    permute(N, nFactor, actualRadix, remainRadix, xRe, xIm, yRe, yIm);

    for (int i=1; i<=nFactor; ++i)
         twiddleTransf(sofarRadix[i], actualRadix[i], remainRadix[i], yRe, yIm);
} // end 'fft'

//---------------------------------------------------------------------------
//  FFT inverse transformation
void cls_fft::ifft(int N, TReal yRe[], TReal yIm[], TReal xRe[], TReal xIm[])
{
    // TODO 2: do at lower level for better performance
    fft(N, yIm, yRe, xIm, xRe);
    for ( int i=0; i<N; ++i )
        {
         xRe[i] /= N;
         xIm[i] /= N;
        }
} // end 'ifft'

//---------------------------------------------------------------------------
// Calculate spectrum |fft|
void cls_fft::spect_fft(int N, TReal xRe[], TReal xIm[], TReal S[])
{
    // TODO 2: do at lower level for better performance
    TReal* yIm = new TReal[N];

    fft(N, xRe, xIm, S, yIm);

    // . . . spectrum
    for ( int i=0; i<N; ++i )
        {
         S[i] = (2.0/N) * std::sqrt(S[i]*S[i] + yIm[i]*yIm[i]);
        }

    delete[] yIm;
} // end 'spect_fft'

//---------------------------------------------------------------------------
//  FFT transformation (real signal x sized N, yRe[] and yIm[] are sized N/2)
void cls_fft::fft(int N, TReal x[], TReal yRe[], TReal yIm[])
{
    // TODO 1: optimize at lower level
    TReal* xIm = new TReal[N];
    for ( int i=0; i<N; ++i ) xIm[i] = 0;
    fft(N, x, xIm, yRe, yIm);
    delete[] xIm;

    /* Splitting real signal into a fictitious half length complex signal
    int N_2 = N/2;
    TReal* hRe = new TReal[N_2];
    TReal* hIm = new TReal[N_2];
    TReal K = 2.0 * M_PI / N;
    TReal coski, sinki;
    int j; // (N/2 - 1) - i

    // . . . fft of fictitious half length complex signal
    fft(N_2, x, x+N_2, hRe, hIm);

    // Now must build real fft
    // . . . Now must retrieve real signal fft
    for ( int i=0; i<N_2; ++i )
        {
         j = (N_2-1) - i;
         coski = std::cos(K*i);
         sinki = std::sin(K*i);
         yRe[i] = hRe[i]+hRe[j] + (hRe[i]-hRe[j])*sinki + (hIm[i]+hIm[j])*coski;
         yIm[i] = hIm[i]-hIm[j] + (hIm[i]+hIm[j])*sinki - (hRe[i]-hRe[j])*coski;
        }

    delete[] hRe;
    delete[] hIm;
*/

} // end 'fft (real signal)'

//---------------------------------------------------------------------------
//  FFT inverse transformation (real signal)
void cls_fft::ifft(int N, TReal yRe[], TReal yIm[], TReal x[])
{
    // TODO 2: optimize at lower level
    TReal* xIm = new TReal[N];

    fft(N, yIm, yRe, xIm, x);

    delete[] xIm; // Ehm, now should contain very small values

    for ( int i=0; i<N; ++i ) x[i] /= N;
} // end 'ifft (real signal)'


//---------------------------------------------------------------------------
// Calculate spectrum |fft| (real signal, S[] must be sized N/2+1)
void cls_fft::spect_fft(int N, TReal x[], TReal S[])
{
    // TODO 2: optimize at lower level
    TReal* xIm = new TReal[N];
    TReal* yRe = new TReal[N];
    TReal* yIm = new TReal[N];
    for ( int i=0; i<N; ++i ) xIm[i] = 0;

    try {
         fft(N, x, xIm, yRe, yIm);

         // . . . spectrum
         for ( int i=0, imax=N/2; i<=imax; ++i )
             {
              S[i] = (2.0/N) * std::sqrt(yRe[i]*yRe[i] + yIm[i]*yIm[i]);
             }
        }
    __finally {
               delete[] xIm;
               delete[] yRe;
               delete[] yIm;
              }
} // end 'spect_fft (real signal)'

/*---------------------------------------------------------------------------
//  FFT inverse transformation (real signal)
void cls_fft::ispect(int N, TReal S[], TReal x[])
{
    // TODO 1: optimize at lower level
    TReal* xIm = new TReal[N];
    TReal* yIm = new TReal[N];
    for ( int i=0; i<N; ++i ) yIm[i] = 0;

    fft(N, yIm, S, xIm, x);

    delete[] xIm; // Ehm, now should contain very small values

    for ( int i=0; i<N; ++i ) x[i] /= N;
} // end 'ifft (real signal)'
*/


//---------------------------------------------------------------------------
// Calculate spectrum |dft| (real signal, S[] must be sized N/2+1)
void cls_fft::spect_dft(int N, TReal x[], TReal S[])
{
/*
           __ N-1         -j (h·k · 2pi/N)
      Yk = >       Xh ·  e
           ¯¯ h=0
*/


// . . . Prepare
    TReal m = 2.0 / double(N);
    TReal w = m * M_PI;
    TReal a; // current angle [rad]
    TReal re,im; // dft real and imaginary parts

// . . . Do
     for ( int k=0, kmax=N/2; k<=kmax; ++k )
         {
          re = im = 0;
          for ( int h=0; h<N; ++h )
              {
               a = w * double(h)*double(k);
               re += x[h] * std::cos(a);
               im += x[h] * std::sin(-a);
              }
          S[k] = m * std::sqrt(re*re + im*im);
         }

} // end 'spect_dft (real signal)'


//---------------------------------------------------------------------------
// Calculate normalized module 2/N * |Re²+Im²|
void cls_fft::norm(int N, TReal Re[], TReal Im[], TReal M[])
{
    for ( int i=0; i<N; ++i )
        {
         M[i] = (2.0/N) * std::sqrt(Re[i]*Re[i] + Im[i]*Im[i]);
        }
} // end 'norm'




//---------------------------- Private methods ------------------------------


//---------------------------------------------------------------------------
// Setup table with sofar, actual, and remainRadix
//  After N is factored the parameters that control the stages are generated.
//  For each stage we have:
//    sofar   : the product of the radices so far.
//    actual  : the radix handled in this stage.
//    remain  : the product of the remaining radices.
void cls_fft::transTableSetup(int sofar[], int actual[], int remain[], int *nFact, int *nPoints)
{
    factorize(*nPoints, nFact, actual);
    if ( actual[1] > maxPrimeFactor )
       {
        throw Exception( String("Prime factor of FFT length too large: ") + String(actual[1]) +
                         String("; modify the value of maxPrimeFactor and recompile") );
       }

    remain[0] = *nPoints;
    sofar[1] = 1;
    remain[1] = *nPoints / actual[1];

    for ( int i=2; i<=*nFact; ++i )
        {
         sofar[i] = sofar[i-1]*actual[i-1];
         remain[i] = remain[i-1] / actual[i];
        }
} // end 'transTableSetup'

//---------------------------------------------------------------------------
// Rotate multiplications and DFT's for one stage
//  Twiddle factor multiplications and transformations are performed on a
//  group of data. The number of multiplications with 1 are reduced by skipping
//  the twiddle multiplication of the first stage and of the first group of the
//  following stages.
void cls_fft::twiddleTransf(int sofarRadix, int radix, int remainRadix, TReal yRe[], TReal yIm[])
{
// . . . Local bags
    TReal  omega, tw_re, tw_im;
    TReal  cosw, sinw, gem;
    TReal  t1_re, t1_im,  t2_re, t2_im,  t3_re, t3_im;
    TReal  t4_re, t4_im,  t5_re, t5_im;
    TReal  m2_re, m2_im,  m3_re, m3_im,  m4_re, m4_im;
    TReal  m1_re, m1_im,  m5_re, m5_im;
    TReal  s1_re, s1_im,  s2_re, s2_im,  s3_re, s3_im;
    TReal  s4_re, s4_im,  s5_re, s5_im;

    static cls_fft::TReal twiddleRe[maxPrimeFactor],
                          twiddleIm[maxPrimeFactor];

    int blockNo;

    int adr = 0,
        groupOffset = 0,
        dataOffset = 0;

// . . . Start
    initTrig(radix);

    omega = 2*M_PI / TReal(sofarRadix*radix);
    cosw = std::cos(omega);
    sinw = -std::sin(omega);
    tw_re = 1.0;
    tw_im = 0;

    for ( int dataNo=0; dataNo<sofarRadix; ++dataNo )
        {
         if ( sofarRadix>1 )
            {
             twiddleRe[0] = 1.0;
             twiddleIm[0] = 0.0;
             twiddleRe[1] = tw_re;
             twiddleIm[1] = tw_im;
             for (int twNo=2; twNo<radix; ++twNo)
                 {
                  twiddleRe[twNo] = tw_re*twiddleRe[twNo-1] - tw_im*twiddleIm[twNo-1];
                  twiddleIm[twNo] = tw_im*twiddleRe[twNo-1] + tw_re*twiddleIm[twNo-1];
                 }
             gem = cosw*tw_re - sinw*tw_im;
             tw_im = sinw*tw_re + cosw*tw_im;
             tw_re = gem;
            } // end 'if ( sofarRadix>1 )'
            
         for ( int groupNo=0; groupNo<remainRadix; ++groupNo )
             {
              if ( (sofarRadix>1) && (dataNo>0) )
                   {
                    zRe[0] = yRe[adr];
                    zIm[0] = yIm[adr];
                    blockNo = 1;
                    do {
                        adr += sofarRadix;
                        zRe[blockNo] = twiddleRe[blockNo] * yRe[adr] - twiddleIm[blockNo] * yIm[adr];
                        zIm[blockNo] = twiddleRe[blockNo] * yIm[adr] + twiddleIm[blockNo] * yRe[adr];
                        ++blockNo;
                       } while (blockNo < radix);
                   }
              else {
                    for ( blockNo=0; blockNo<radix; ++blockNo )
                        {
                         zRe[blockNo] = yRe[adr];
                         zIm[blockNo] = yIm[adr];
                         adr += sofarRadix;
                        }
                   }

              switch ( radix )
                     {
                      case  2  : gem=zRe[0] + zRe[1];
                                 zRe[1]=zRe[0] -  zRe[1]; zRe[0]=gem;
                                 gem=zIm[0] + zIm[1];
                                 zIm[1]=zIm[0] - zIm[1]; zIm[0]=gem;
                                 break;
                      case  3  : t1_re=zRe[1] + zRe[2]; t1_im=zIm[1] + zIm[2];
                                 zRe[0]=zRe[0] + t1_re; zIm[0]=zIm[0] + t1_im;
                                 m1_re=c3_1*t1_re; m1_im=c3_1*t1_im;
                                 m2_re=c3_2*(zIm[1] - zIm[2]);
                                 m2_im=c3_2*(zRe[2] -  zRe[1]);
                                 s1_re=zRe[0] + m1_re; s1_im=zIm[0] + m1_im;
                                 zRe[1]=s1_re + m2_re; zIm[1]=s1_im + m2_im;
                                 zRe[2]=s1_re - m2_re; zIm[2]=s1_im - m2_im;
                                 break;
                      case  4  : t1_re=zRe[0] + zRe[2]; t1_im=zIm[0] + zIm[2];
                                 t2_re=zRe[1] + zRe[3]; t2_im=zIm[1] + zIm[3];

                                 m2_re=zRe[0] - zRe[2]; m2_im=zIm[0] - zIm[2];
                                 m3_re=zIm[1] - zIm[3]; m3_im=zRe[3] - zRe[1];

                                 zRe[0]=t1_re + t2_re; zIm[0]=t1_im + t2_im;
                                 zRe[2]=t1_re - t2_re; zIm[2]=t1_im - t2_im;
                                 zRe[1]=m2_re + m3_re; zIm[1]=m2_im + m3_im;
                                 zRe[3]=m2_re - m3_re; zIm[3]=m2_im - m3_im;
                                 break;
                      case  5  : t1_re=zRe[1] + zRe[4]; t1_im=zIm[1] + zIm[4];
                                 t2_re=zRe[2] + zRe[3]; t2_im=zIm[2] + zIm[3];
                                 t3_re=zRe[1] - zRe[4]; t3_im=zIm[1] - zIm[4];
                                 t4_re=zRe[3] - zRe[2]; t4_im=zIm[3] - zIm[2];
                                 t5_re=t1_re + t2_re; t5_im=t1_im + t2_im;
                                 zRe[0]=zRe[0] + t5_re; zIm[0]=zIm[0] + t5_im;
                                 m1_re=c5_1*t5_re; m1_im=c5_1*t5_im;
                                 m2_re=c5_2*(t1_re - t2_re);
                                 m2_im=c5_2*(t1_im - t2_im);

                                 m3_re=-c5_3*(t3_im + t4_im);
                                 m3_im=c5_3*(t3_re + t4_re);
                                 m4_re=-c5_4*t4_im; m4_im=c5_4*t4_re;
                                 m5_re=-c5_5*t3_im; m5_im=c5_5*t3_re;

                                 s3_re=m3_re - m4_re; s3_im=m3_im - m4_im;
                                 s5_re=m3_re + m5_re; s5_im=m3_im + m5_im;
                                 s1_re=zRe[0] + m1_re; s1_im=zIm[0] + m1_im;
                                 s2_re=s1_re + m2_re; s2_im=s1_im + m2_im;
                                 s4_re=s1_re - m2_re; s4_im=s1_im - m2_im;

                                 zRe[1]=s2_re + s3_re; zIm[1]=s2_im + s3_im;
                                 zRe[2]=s4_re + s5_re; zIm[2]=s4_im + s5_im;
                                 zRe[3]=s4_re - s5_re; zIm[3]=s4_im - s5_im;
                                 zRe[4]=s2_re - s3_re; zIm[4]=s2_im - s3_im;
                                 break;
                      case  8  : fft_8(); break;
                      case 10  : fft_10(); break;
                      default  : fft_odd(radix); break;
              } // end 'switch radix'

              adr = groupOffset;
              for ( blockNo=0; blockNo<radix; ++blockNo )
                  {
                   yRe[adr] = zRe[blockNo];
                   yIm[adr] = zIm[blockNo];
                   adr += sofarRadix;
                  }
              groupOffset += sofarRadix*radix;
              adr = groupOffset;
             } // end 'for groupNo'

         adr = groupOffset = ++dataOffset;
        } // end 'for DataNo'
} // end 'twiddleTransf'


//---------------------------------------------------------------------------
// Factor the transformation length
// Factor the length of the DFT, N, into
// factors efficiently handled by the routines
void cls_fft::factorize(int N, int *nFact, int fact[])
{
    int i,j;
    const int radices_size = 6;
    const int radices[radices_size] = {2,3,4,5,8,10};
    int factors[maxFactorCount];

// . . . Banal case
    if ( N == 1 ) factors[1] = j = 1;
    else j = 0;

// . . . Get factors
    i = radices_size-1;
    while ( (N>1) && (i>=0) )
          {
           if ( (N % radices[i]) == 0 )
                {
                 N = N / radices[i];
                 ++j;
                 factors[j] = radices[i];
                }
           else --i;
          }

// . . . substitute factors 2*8 with 4*4
    if ( factors[j] == 2 )   
       {
        i = j - 1;
        while ( (i>0) && (factors[i] != 8) ) --i;
        if ( i > 0 )
           {
            factors[j] = 4;
            factors[i] = 4;
		   }
       }

    if ( N > 1 )
       {
		for ( int k=2, kmax=std::sqrt(double(N))+1; k<kmax; ++k )
            {
             while ( (N%k) == 0 )
                   {
                    N = N / k;
                    ++j;
                    factors[j] = k;
                   }
            }
        if ( N > 1 )
           {
            ++j;
            factors[j] = N;
           }
       }

    for ( i=1; i<=j; ++i )
        {
         fact[i] = factors[j-i+1];
        }
    *nFact = j;
} // end 'factorize'



//---------------------------------------------------------------------------
// Permutation allows in-place calculations
//  The sequence y is the permuted input sequence x so that the following
//  transformations can be performed in-place, and the final result is
//  in the correct order
void cls_fft::permute(int nPoint, int nFact, int fact[], int remain[],
                      TReal xRe[], TReal xIm[], TReal yRe[], TReal yIm[] )
{
    int j;
    int count[maxFactorCount];

    for (int i=1; i<=nFact; ++i) count[i] = 0;
    int k = 0;
    for (int i=0, imax=nPoint-2; i<=imax; ++i)
        {
         yRe[i] = xRe[k];
         yIm[i] = xIm[k];
         j = 1;
         k += remain[j];
         ++count[1];
         while ( count[j] >= fact[j] )
               {
                count[j] = 0;
                k -= remain[j-1] - remain[j+1];
                ++j;
                ++count[j];
               }
        }
    yRe[nPoint-1] = xRe[nPoint-1];
    yIm[nPoint-1] = xIm[nPoint-1];
} // end 'permute'




//---------------------------------------------------------------------------
// Initialise sine/cosine table
void cls_fft::initTrig( int radix )
{
    TReal w,xre,xim;

    w = 2*M_PI / radix;
    trigRe[0] = 1;
    trigIm[0] = 0;
    xre = std::cos(w);
    xim = -std::sin(w);
    trigRe[1] = xre;
    trigIm[1] = xim;
    for ( int i=2; i<radix; ++i )
       {
        trigRe[i] = xre*trigRe[i-1] - xim*trigIm[i-1];
        trigIm[i] = xim*trigRe[i-1] + xre*trigIm[i-1];
       }
} // end 'initTrig'


//---------------------------------------------------------------------------
// A number of short DFT's are implemented with a minimum of
// arithmetical operations and using (almost) straight line code
// resulting in very fast execution when the factors of N belong
// to this set. Especially radix-10 is optimized.

//---------------------------------------------------------------------------
// Length 4 DFT, a la Nussbaumer
inline void cls_fft::fft_4(TReal aRe[], TReal aIm[])
{
    TReal  t1_re, t1_im,  t2_re, t2_im;
    TReal  m2_re, m2_im,  m3_re, m3_im;

    t1_re = aRe[0] + aRe[2];  t1_im = aIm[0] + aIm[2];
    t2_re = aRe[1] + aRe[3];  t2_im = aIm[1] + aIm[3];

    m2_re = aRe[0] - aRe[2];  m2_im = aIm[0] - aIm[2];
    m3_re = aIm[1] - aIm[3];  m3_im = aRe[3] - aRe[1];

    aRe[0] = t1_re + t2_re;   aIm[0] = t1_im + t2_im;
    aRe[2] = t1_re - t2_re;   aIm[2] = t1_im - t2_im;
    aRe[1] = m2_re + m3_re;   aIm[1] = m2_im + m3_im;
    aRe[3] = m2_re - m3_re;   aIm[3] = m2_im - m3_im;
} // end 'fft_4'

//---------------------------------------------------------------------------
// length 5 DFT, a la Nussbaumer
inline void cls_fft::fft_5(TReal aRe[], TReal aIm[])
{    
    TReal t1_re, t1_im,  t2_re, t2_im,  t3_re, t3_im;
    TReal t4_re, t4_im,  t5_re, t5_im;
    TReal m2_re, m2_im,  m3_re, m3_im,  m4_re, m4_im;
    TReal m1_re, m1_im,  m5_re, m5_im;
    TReal s1_re, s1_im,  s2_re, s2_im,  s3_re, s3_im;
    TReal s4_re, s4_im,  s5_re, s5_im;

    t1_re = aRe[1] + aRe[4];     t1_im = aIm[1] + aIm[4];
    t2_re = aRe[2] + aRe[3];     t2_im = aIm[2] + aIm[3];
    t3_re = aRe[1] - aRe[4];     t3_im = aIm[1] - aIm[4];
    t4_re = aRe[3] - aRe[2];     t4_im = aIm[3] - aIm[2];
    t5_re = t1_re + t2_re;       t5_im = t1_im + t2_im;
    aRe[0] = aRe[0] + t5_re;     aIm[0] = aIm[0] + t5_im;
    m1_re = c5_1*t5_re;          m1_im = c5_1*t5_im;
    m2_re = c5_2*(t1_re-t2_re);  m2_im = c5_2*(t1_im-t2_im);

    m3_re = -c5_3*(t3_im + t4_im); m3_im = c5_3*(t3_re + t4_re);
    m4_re = -c5_4*t4_im;           m4_im = c5_4*t4_re;
    m5_re = -c5_5*t3_im;           m5_im = c5_5*t3_re;

    s3_re = m3_re - m4_re;   s3_im = m3_im - m4_im;
    s5_re = m3_re + m5_re;   s5_im = m3_im + m5_im;
    s1_re = aRe[0] + m1_re;  s1_im = aIm[0] + m1_im;
    s2_re = s1_re + m2_re;   s2_im = s1_im + m2_im;
    s4_re = s1_re - m2_re;   s4_im = s1_im - m2_im;

    aRe[1] = s2_re + s3_re;  aIm[1] = s2_im + s3_im;
    aRe[2] = s4_re + s5_re;  aIm[2] = s4_im + s5_im;
    aRe[3] = s4_re - s5_re;  aIm[3] = s4_im - s5_im;
    aRe[4] = s2_re - s3_re;  aIm[4] = s2_im - s3_im;
} // end 'fft_5'

//---------------------------------------------------------------------------
// length 8 DFT, a la Nussbaumer
inline void cls_fft::fft_8()
{
    TReal  aRe[4], aIm[4], bRe[4], bIm[4], gem;

    aRe[0] = zRe[0];    bRe[0] = zRe[1];
    aRe[1] = zRe[2];    bRe[1] = zRe[3];
    aRe[2] = zRe[4];    bRe[2] = zRe[5];
    aRe[3] = zRe[6];    bRe[3] = zRe[7];

    aIm[0] = zIm[0];    bIm[0] = zIm[1];
    aIm[1] = zIm[2];    bIm[1] = zIm[3];
    aIm[2] = zIm[4];    bIm[2] = zIm[5];
    aIm[3] = zIm[6];    bIm[3] = zIm[7];

    fft_4(aRe, aIm); fft_4(bRe, bIm);

    gem    = c8*(bRe[1] + bIm[1]);
    bIm[1] = c8*(bIm[1] - bRe[1]);
    bRe[1] = gem;
    gem    =  bIm[2];
    bIm[2] = -bRe[2];
    bRe[2] = gem;
    gem    =  c8*(bIm[3] - bRe[3]);
    bIm[3] = -c8*(bRe[3] + bIm[3]);
    bRe[3] = gem;
    
    zRe[0] = aRe[0] + bRe[0];   zRe[4] = aRe[0] - bRe[0];
    zRe[1] = aRe[1] + bRe[1];   zRe[5] = aRe[1] - bRe[1];
    zRe[2] = aRe[2] + bRe[2];   zRe[6] = aRe[2] - bRe[2];
    zRe[3] = aRe[3] + bRe[3];   zRe[7] = aRe[3] - bRe[3];

    zIm[0] = aIm[0] + bIm[0];   zIm[4] = aIm[0] - bIm[0];
    zIm[1] = aIm[1] + bIm[1];   zIm[5] = aIm[1] - bIm[1];
    zIm[2] = aIm[2] + bIm[2];   zIm[6] = aIm[2] - bIm[2];
    zIm[3] = aIm[3] + bIm[3];   zIm[7] = aIm[3] - bIm[3];
} // end 'fft_8'

//---------------------------------------------------------------------------
// length 10 DFT using prime factor FFT
inline void cls_fft::fft_10()
{
    TReal  aRe[5], aIm[5], bRe[5], bIm[5];

    aRe[0] = zRe[0];    bRe[0] = zRe[5];
    aRe[1] = zRe[2];    bRe[1] = zRe[7];
    aRe[2] = zRe[4];    bRe[2] = zRe[9];
    aRe[3] = zRe[6];    bRe[3] = zRe[1];
    aRe[4] = zRe[8];    bRe[4] = zRe[3];

    aIm[0] = zIm[0];    bIm[0] = zIm[5];
    aIm[1] = zIm[2];    bIm[1] = zIm[7];
    aIm[2] = zIm[4];    bIm[2] = zIm[9];
    aIm[3] = zIm[6];    bIm[3] = zIm[1];
    aIm[4] = zIm[8];    bIm[4] = zIm[3];

    fft_5(aRe, aIm); fft_5(bRe, bIm);

    zRe[0] = aRe[0] + bRe[0];   zRe[5] = aRe[0] - bRe[0];
    zRe[6] = aRe[1] + bRe[1];   zRe[1] = aRe[1] - bRe[1];
    zRe[2] = aRe[2] + bRe[2];   zRe[7] = aRe[2] - bRe[2];
    zRe[8] = aRe[3] + bRe[3];   zRe[3] = aRe[3] - bRe[3];
    zRe[4] = aRe[4] + bRe[4];   zRe[9] = aRe[4] - bRe[4];

    zIm[0] = aIm[0] + bIm[0];   zIm[5] = aIm[0] - bIm[0];
    zIm[6] = aIm[1] + bIm[1];   zIm[1] = aIm[1] - bIm[1];
    zIm[2] = aIm[2] + bIm[2];   zIm[7] = aIm[2] - bIm[2];
    zIm[8] = aIm[3] + bIm[3];   zIm[3] = aIm[3] - bIm[3];
    zIm[4] = aIm[4] + bIm[4];   zIm[9] = aIm[4] - bIm[4];
} // end 'fft_10'

//---------------------------------------------------------------------------
// Length N DFT, N odd
// Prime factors, that are not in the set of short DFT's are
// handled with direct evaluation of the DFP expression
void cls_fft::fft_odd( int radix )
{
    TReal  rere, reim, imre, imim;
    int j,k;

    static cls_fft::TReal vRe[maxPrimeFactorDiv2], vIm[maxPrimeFactorDiv2],
                          wRe[maxPrimeFactorDiv2], wIm[maxPrimeFactorDiv2];

    int n = radix;
    int nhalf = (n + 1)/2;

    for ( j=1; j<nhalf; ++j )
        {
         vRe[j] = zRe[j] + zRe[n-j];
         vIm[j] = zIm[j] - zIm[n-j];
         wRe[j] = zRe[j] - zRe[n-j];
         wIm[j] = zIm[j] + zIm[n-j];
        }

    for ( j=1; j<nhalf; ++j )
        {
         zRe[j] = zRe[0];
         zIm[j] = zIm[0];
         zRe[n-j] = zRe[0];
         zIm[n-j] = zIm[0];
         k = j;
         for ( int i=1; i<nhalf; ++i )
             {
              rere = trigRe[k] * vRe[i];
              imim = trigIm[k] * vIm[i];
              reim = trigRe[k] * wIm[i];
              imre = trigIm[k] * wRe[i];

              zRe[n-j] += rere + imim;
              zIm[n-j] += reim - imre;
              zRe[j]   += rere - imim;
              zIm[j]   += reim + imre;

              k += j;
              if (k >= n)  k = k - n;
             }
        }
    for ( j=1; j<nhalf; ++j )
        {
         zRe[0] += vRe[j];
         zIm[0] += wIm[j];
        }
} // end 'fft_odd'



//=============================== End of file ===============================

