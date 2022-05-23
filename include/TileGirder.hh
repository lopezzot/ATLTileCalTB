#ifndef TileGirder_h
#define TileGirder_h 1

#include <vector>
#include <string>

class TileGirder
{
public:
        TileGirder              (std::string s)         { m_comment=s; }

        void    comment         (std::string s)         { m_comment=s; }
        void    nelements       (int i)                 { m_nelements=i; }
        void    rposition       (int n, double *a)      { for (int i=0;i<n;i++) m_rposition.push_back(*(a+i)); }
        void    deltar          (int n, double *a)      { for (int i=0;i<n;i++) m_deltar.push_back(*(a+i)); }
        void    width           (int n, double *a)      { for (int i=0;i<n;i++) m_width.push_back(*(a+i)); }
        void    offset          (int n, double *a)      { for (int i=0;i<n;i++) m_offset.push_back(*(a+i)); }
        void    rposition       (double a, int n)       { m_rposition[n]=a; }
        void    deltar          (double a, int n)       { m_deltar[n]=a; }
        void    width           (double a, int n)       { m_width[n]=a; }
        void    offset          (double a, int n)       { m_offset[n]=a; }

        std::string  comment    ()              const   { return m_comment; }
        int     nelements       ()              const   { return m_nelements; }
        double  rposition       (int n)         const   { return m_rposition[n]; }
        double  deltar          (int n)         const   { return m_deltar[n]; }
        double  width           (int n)         const   { return m_width[n]; }
        double  offset          (int n)         const   { return m_offset[n]; }

private:
        std::string             m_comment;
        int                     m_nelements;    // Nr. of girder elements 
        std::vector<double>     m_rposition;    // Radius of the center
        std::vector<double>     m_deltar;       // delta-r of each element
        std::vector<double>     m_width;        // width of each element
        std::vector<double>     m_offset;       // offset
};

#endif
