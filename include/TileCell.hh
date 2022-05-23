#ifndef TileCell_h
#define TileCell_h 1

#include <vector>
#include <string>

class TileCell
{
public:
        TileCell                ()                      { m_nrows=m_nscintillators=0; }
        TileCell                (std::string s)         { m_comment=s; m_nrows=m_nscintillators=0; }

        void    comment         (std::string s)         { m_comment=s; }

        void    scin2sample     (int length, int *array);
        void    scin2cell       (int nrows, int nper, int **array);

        std::string  comment    ()              const   { return m_comment; }
        int     nrows           ()              const   { return m_nrows; }
        int     nperiods        ()              const
        { 
            if (m_nrows>0) { return m_nscintillators/m_nrows; }
            else           { return m_nscintillators; }
        }
        int     nscintillators  ()              const   { return m_nscintillators; }
        int     ncells          (int sample)    const   { return m_ncells[sample]; }
        int     scin2sample     (int scin)      const;
        int     scin2cell       (int per, int scin) const;

private:
        std::string             m_comment;
        int                     m_nrows;          // Nr. of rows in a module
        int                     m_nscintillators; // Nr. of scintillators in a module
        std::vector<int>        m_ncells;         // Nr. of cells in each sample
        std::vector<int>        m_scin2sample;    // m_nrows elements
        std::vector<int>        m_scin2cell;      // m_nscintillators elements 
};

#endif
