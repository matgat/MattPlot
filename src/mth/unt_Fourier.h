//---------------------------------------------------------------------------
#ifndef unt_FourierH
#define unt_FourierH


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *
    *    unt_Mixfft    ver: 0.3
    *    2008 - Matteo Gattanini (matteo.gattanini@gmail.com)
    *
    *    fft part is a C++ readaptation of 'Mixfft.c'
    *    "Arbitrary Length FFT" by Jens Joergen Nielsen
    *    (jjn@get2net.dk)   http://home.get2net.dk/jjn
    *    Same licence applies (free for non-commercial use,
    *    otherwise 100$ to Jens, please contact him)
    *
    *    Example of usage:
    *    --------------------------------------------
    *    #include "unt_Mixfft.h"
    *    double* Re_Xt = new double[N]; // time domain
    *    double* Im_Xt = new double[N];
    *    double* Re_Xf = new double[N]; // freq domain
    *    double* Im_Xf = new double[N];
    *    // ...Fill Xt
    *    cls_fft::fft(N,Re_Xt,Im_Xt,Re_Xf,Im_Xf);
    *    // Now Re_Xf,Im_Xf contains Fourier transform
    *    cls_fft::ifft(N,Re_Xt,Im_Xt,Re_Xf,Im_Xf);
    *    delete[] Re_Xt;
    *    delete[] Im_Xt;
    *    delete[] Re_Xf;
    *    delete[] Im_Xf;
    *
    *    Remarks
    *    --------------------------------------------
    *    *) Not thread safe! should avoid static
    *       variables and file scope bags
    *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * */


////////////////////////////////// cls_fft //////////////////////////////////
//---------------------------------------------------------------------------
class cls_fft { // Encapsulates fft algorithm, could be just a namespace

public://                                                            --------
// . . . Types
    typedef double TReal;

// . . . Public methods
    cls_fft() {;}
    //~cls_fft(){}

    // . . . Main
    static void fft(int N, TReal Re_t[], TReal Im_t[], TReal Re_f[], TReal Im_f[]); // Perform the fft transformation
    static void ifft(int N, TReal Re_t[], TReal Im_t[], TReal Re_f[], TReal Im_f[]); // inverse transformation
    static void spect_fft(int N, TReal Re_t[], TReal Im_t[], TReal S_f[]); // Calculate spectrum |fft|
    // TODO: dft

    // Optimized for real signals
    static void fft(int N, TReal t[], TReal Re_f[], TReal Im_f[]);
    static void ifft(int N, TReal Re_f[], TReal Im_f[], TReal t[]);
    static void spect_fft(int N, TReal t[], TReal S_f[]);
    //static void ispect(int N, TReal S_f[], TReal t[]);

    static void spect_dft(int N, TReal t[], TReal S_f[]);

    // . . . Facilities
    static void norm(int N, TReal Re[], TReal Im[], TReal M[]); // Calculate normalized module 2/N * |Re²+Im²|

// . . . Public attributes (!)

private://                                                           --------
// . . . Private attributes

// . . . Private methods
    static void factorize(int, int*, int[]);
    static void permute(int, int, int[], int[], TReal[], TReal[], TReal[], TReal[] );
    static void initTrig(int);
    static inline void fft_4(TReal aRe[], TReal aIm[]);
    static inline void fft_5(TReal aRe[], TReal aIm[]);
    static inline void fft_8();
    static inline void fft_10();
    static void fft_odd(int);
    static void transTableSetup(int[], int[], int[], int*, int*);
    static void twiddleTransf(int, int, int, TReal[], TReal[]);
}; // end 'cls_fft'

//---------------------------------------------------------------------------
#endif