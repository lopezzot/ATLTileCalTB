#ifndef TileGeometry_h
#define TileGeometry_h 1

#include <vector>
#include <string>
#include <iostream.h>

#include "TileSection.hh"

class TileGeometry
{
public:
        TileGeometry            (std::string s)         { m_comment=s; m_section_in_use=-1; }

        void    comment         (std::string s)         { m_comment=s; }
        void    version         (int i)                 { m_version=i; }
        void    rmin            (double r)              { m_rmin=r; }
        void    rmax            (double r)              { m_rmax=r; }
        void    zmax            (double r)              { m_zmax=r; }
        void    use_section     (int i)                 { m_section_in_use=i; }

        std::string  comment    ()              const   { return m_comment; }
        int     version         ()              const   { return m_version; }
        double  rmin            ()              const   { return m_rmin; }
        double  rmax            ()              const   { return m_rmax; }
        double  zmax            ()              const   { return m_zmax; }
        int     nsections       ()              const   { return m_sectionlist.size(); }
        void    section         (TileSection *s) 
        {
            m_sectionlist.push_back(s);
            m_nsections=m_sectionlist.size();
        }
        TileSection* section    (int i)         const
        {
            if (i<0) return section();
            if (i<(int)m_sectionlist.size()) return m_sectionlist[i];
            else return 0;
        }
        TileSection* section    ()              const
        {
            if (m_section_in_use<0) {
                G4cout<<"No section selected!!!"<<endl;
                return 0;
            }
            else if (m_section_in_use<(int)m_sectionlist.size()) {
                return m_sectionlist[m_section_in_use];
            } else {
                G4cout<<"Section in use is too big!!!"<<endl;
                return 0;
            }
        }

private:
        std::string     m_comment;
        int             m_version;
        int             m_nsections;
        double          m_rmin;
        double          m_rmax;
        double          m_zmax;
        int             m_section_in_use;
        std::vector<TileSection*>       m_sectionlist;
};

#endif
