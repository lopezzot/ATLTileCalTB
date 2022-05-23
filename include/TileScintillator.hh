#ifndef TileScintillator_h
#define TileScintillator_h 1

#include <vector>
#include <string>

class TileScintillator
{
public:
	TileScintillator        (std::string s)         { m_comment=s; }

        void    comment         (std::string s)         { m_comment=s; }
	void    nscintillators  (int i)                 { m_nscintillators=i; }
	void    thickness       (double t)              { m_thickness=t; }
        void    rwrapp          (double rw)             { m_rwrapp=rw; }
        void    thickwrapp      (double tw)             { m_thickwrapp=tw; }
	void    heigth          (int n, double *a)      { for (int i=0;i<n;i++) m_heigth.push_back(*(a+i)); }
	void    rposition       (int n, double *a)      { for (int i=0;i<n;i++) m_rposition.push_back(*(a+i)); }
        void    zposition       (int n, double *a)      { for (int i=0;i<n;i++) m_zposition.push_back(*(a+i)); }
        void    heigth          (double a,int n)        { m_heigth[n]=a; }
        void    rposition       (double a,int n)        { m_rposition[n]=a; }
        void    zposition       (double a,int n)        { m_zposition[n]=a; }

	std::string  comment    ()              const   { return m_comment; }
	int     nscintillators  ()              const   { return m_nscintillators; }
	double  thickness       ()              const   { return m_thickness; }
        double  rwrapp          ()              const   { return m_rwrapp; }
        double  thickwrapp      ()              const   { return m_thickwrapp; }

	double  heigth          (int n)         const   { return m_heigth[n]; }
	double  rposition       (int n)         const   { return m_rposition[n]; }
	double  zposition       (int n)         const   { return m_zposition[n]; }

private:
	std::string             m_comment;
	int                     m_nscintillators; // Nr. of Scintillators in a period
	std::vector<double>     m_heigth;         // Heigth of each scintillator
	std::vector<double>     m_rposition;      // positions in R
	std::vector<double>     m_zposition;      // positions in Z
	double                  m_thickness;      // thickness
	double                  m_rwrapp;         // radial tile wrapping
	double                  m_thickwrapp;     // tile wrapping thickness
};

#endif

