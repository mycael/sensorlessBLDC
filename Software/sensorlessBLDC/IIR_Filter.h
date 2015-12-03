
#ifndef MDS_IIR_H
#define MDS_IIR_H

/* .......................................................................... */

typedef struct
{
  int  numSectionsLess1;
  int *pCoefs;
  int  psvpage;
  int *pStates;
  int  initialGain;
  int  finalShift;
} IIRCanonicFilter;

typedef struct
{
  int  numSectionsLess1;
  int *pCoefs;
  int  psvpage;
  int *pStates1;
  int *pStates2;
  int  finalShift;
} IIRTransposeFilter;

extern void BlockIIRCanonicFilter( IIRCanonicFilter *, int *, int *, int );
extern void IIRCanonicFilterInit( IIRCanonicFilter *pFilter );

extern void BlockIIRTransposeFilter( IIRTransposeFilter *, int *, int *, int );
extern void IIRTransposeFilterInit( IIRTransposeFilter *);

/* .......................................................................... */

#endif /* MDS_IIR_H */
