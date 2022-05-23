#include "TileCell.hh"

void TileCell::scin2sample(int length, int *array) {
    m_nrows=length;
    m_scin2sample.reserve(m_nrows);
    for (int i=0; i<length; ++i) {
        m_scin2sample.push_back(*(array+i));
    }
}

void TileCell::scin2cell(int nrows, int nper, int **array) {
    m_nscintillators = nrows*nper;
    m_scin2cell.reserve(m_nscintillators);
    int row,per;
    int *cellnum = new int[nrows];
    int *scinnum = new int[nrows];
    for (row=0; row<nrows; ++row) {
        cellnum[row] = 0; // current cell number in each row
        scinnum[row] = array[row][0]; // cell boundary
    }
    for (per=0; per<nper; ++per) {
        for (row=0; row<nrows; ++row) {
            if (per == scinnum[row] ) { // we reached boundary
                ++cellnum[row]; // next cell number
                scinnum[row] += array[row][cellnum[row]]; // next boundary
            }
            m_scin2cell.push_back(cellnum[row]);
        }
    }
    m_ncells.resize(nrows);
    for (row=0; row<m_nrows; ++row) {
        m_ncells[scin2sample(row)] = cellnum[row]+1;
    }
    delete[] cellnum;
    delete[] scinnum;
}

int  TileCell::scin2sample(int scin) const {
    if (scin < m_nrows) {
        return m_scin2sample[scin];
    } else {
        return scin;
    }
}

int  TileCell::scin2cell(int per, int scin) const {
    if (scin < m_nrows && per*m_nrows+scin < m_nscintillators) {
        return m_scin2cell[per*m_nrows+scin];
    } else {
        return per;
    }
}

