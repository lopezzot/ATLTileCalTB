#ifndef TileSection_h
#define TileSection_h 1

#include <string>

#include "TileCell.hh"
#include "TileGirder.hh"
#include "TileScintillator.hh"

class TileSection
{
public:
	TileSection             ()                      { m_girder=0; m_scintillator=0; m_cell=0; }
	TileSection             (std::string s)         { m_comment=s; m_girder=0; m_scintillator=0; m_cell=0; }

	void    comment         (std::string s)         { m_comment=s; }
	void    nmodules        (int i)                 { m_nmodules=i; }
	void    nperiods        (int i)                 { m_nperiods=i; }
	void    offset          (double r)              { m_offset=r; }
	void    rmin            (double r)              { m_rmin=r; }
	void    rmax            (double r)              { m_rmax=r; }
	void    periodthickness (double t)              { m_periodthickness=t; }
	void    drfront         (double t)              { m_drfront=t; }
	void    dzend           (double t)              { m_dzend=t; }
	void    flangex         (double t)              { m_flangex=t; }
	void    flangey         (double t)              { m_flangey=t; }
	void    phigap          (double t)              { m_phigap=t; }
	void    tilecell        (TileCell *s)           { m_cell=s; }
	void    tilegirder      (TileGirder* g)         { m_girder=g; }
	void    tilescintillator(TileScintillator *s)   { m_scintillator=s; }

	std::string  comment    ()              const   { return m_comment; }
	int     nmodules        ()              const   { return m_nmodules; }
	int     nperiods        ()              const   { return m_nperiods; }
	double  offset          ()              const   { return m_offset; }
	double  rmin            ()              const   { return m_rmin; }
	double  rmax            ()              const   { return m_rmax; }
	double  periodthickness ()              const   { return m_periodthickness; }
	double  drfront         ()              const   { return m_drfront; }
	double  dzend           ()              const   { return m_dzend; }
	double  flangex         ()              const   { return m_flangex; }
	double  flangey         ()              const   { return m_flangey; }
	double  phigap          ()              const   { return m_phigap; }
	TileCell*          tilecell         ()  const   { return m_cell; }
	TileGirder*        tilegirder       ()  const   { return m_girder; }
	TileScintillator*  tilescintillator ()  const   { return m_scintillator; }

private:
	std::string     m_comment;
	int             m_nmodules;
	int             m_nperiods;
	double          m_offset;
	double          m_rmin;
	double          m_rmax;
	double          m_periodthickness;
	double          m_drfront;
	double          m_dzend;
	double          m_flangex;
	double          m_flangey;
	double          m_phigap;
	TileCell*               m_cell;
	TileGirder*             m_girder;
	TileScintillator*       m_scintillator;
};

#endif
