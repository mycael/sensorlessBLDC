extern IIRTransposeFilter BEMF_phaseA_Filter;
extern IIRTransposeFilter BEMF_phaseB_Filter;
extern IIRTransposeFilter BEMF_phaseC_Filter;
extern int BEMF_filterCoefs_81940Hz;
extern int BEMF_filterCoefs_49152Hz;

/* The following C-code fragment demonstrates how to call the filter routine
#include "IIR_Filter.h"
#include "BEMF_filter.h"

// NUM_SAMPLES defines the number of samples in one block of input data.
// This value should be changed as needed for the application
#define NUM_SAMPLES 100

{
   // Declare input and output sample arrays.
   int  inSamples[NUM_SAMPLES], outSamples[NUM_SAMPLES];


   // Call the IIRTransposeFilterInit routine to zero out the state variables
   IIRTransposeFilterInit( &BEMF_filterFilter );

   // Call BlockIIRTransposeFilter for each block of input samples
   // This routine would normally be called inside a FOR or a DO-WHILE loop
   // Only one instance has been shown
   BlockIIRTransposeFilter( &BEMF_filterFilter, &inSamples[0], &outSamples[0], NUM_SAMPLES );
}
*/
