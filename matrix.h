#ifndef _MATRIX_H_
#define _MATRIX_H_

#include<vector>
#include<memory>
#include<cassert>

#include"matrix_fwd.h"
#include"iterators.h"
#include "multiplication_proxy.h"

//FUNCTIONS

//overloads for the sum
template<class T,class U, class dec, class dec2>
matrix<decltype(T()+U())> operator+ (const matrix_ref<T,dec>& m, const matrix_ref<U,dec2>& m2){
	matrix<decltype(U()+T())> result(m.get_height(),m.get_width());
	for (int i = 0; i < m.get_height(); ++i) {
		for (int j = 0; j < m.get_width() ; ++j) {
			result(i,j)=m(i,j)+m2(i,j);
		}
	}
	return result;
}


template<class T, class U>
matrix<decltype(T()+U())> operator+ (const matrix_wrap<U>& m, const matrix_wrap<T>& m2){
	matrix<decltype(U()+T())> result(m.get_height(),m.get_width());
	for (int i = 0; i < m.get_height(); ++i) {
		for (int j = 0; j < m.get_width() ; ++j) {
			result(i,j)=m(i,j)+m2(i,j);
		}
	}
	return result;
}

template<class T,class U, class dec>
matrix<decltype(T()+U())> operator+ (const matrix_ref<T,dec>& m, const matrix_wrap<T>& m2){
	matrix<decltype(U()+T())> result(m.get_height(),m.get_width());
	for (int i = 0; i < m.get_height(); ++i) {
		for (int j = 0; j < m.get_width() ; ++j) {
			result(i,j)=m(i,j)+m2(i,j);
		}
	}
	return result;
}

template<class T,class U, class dec>
matrix<decltype(T()+U())> operator+ (const matrix_wrap<T>& m, const matrix_ref<T,dec>& m2){
	matrix<decltype(U()+T())> result(m.get_height(),m.get_width());
	for (int i = 0; i < m.get_height(); ++i) {
		for (int j = 0; j < m.get_width() ; ++j) {
			result(i,j)=m(i,j)+m2(i,j);
		}
	}
	return result;
}

//overloads for the product

//CLASSES
template<typename T> 
class matrix_ref<T, Plain>{
	public:
	
	//type members
	typedef T type;
	typedef Plain matrix_type;
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;
	typedef typename std::vector<T>::iterator row_iterator;
	typedef typename std::vector<T>::const_iterator const_row_iterator;
	
	typedef index_col_iterator<T,Plain> col_iterator;
	typedef const_index_col_iterator<T,Plain> const_col_iterator;
	
	
	T& operator ()( unsigned row, unsigned column ) {
		return data->operator[](row*width + column);
	}
	const T& operator ()( unsigned row, unsigned column ) const { 
		return data->operator[](row*width + column);
	}
	
	
	
	iterator begin() { return data->begin(); }
	iterator end() { return data->end(); }
	const_iterator begin() const { return data->begin(); }
	const_iterator end() const { return data->end(); }
	
	row_iterator row_begin(unsigned i) { return data->begin() + i*width; }
	row_iterator row_end(unsigned i) { return data->begin() + (i+1)*width; }
	const_row_iterator row_begin(unsigned i) const { return data->begin() + i*width; }
	const_row_iterator row_end(unsigned i) const { return data->begin() + (i+1)*width; }
	
	col_iterator col_begin(unsigned i) { return col_iterator(*this,0,i); }
	col_iterator col_end(unsigned i) { return col_iterator(*this,0,i+1); }
	const_col_iterator col_begin(unsigned i) const { return const_col_iterator(*this,0,i); }
	const_col_iterator col_end(unsigned i) const { return const_col_iterator(*this,0,i+1); }
	
	
	matrix_ref<T, Transpose<Plain>> transpose() const { 
		return matrix_ref<T, Transpose<Plain>>(*this);
	}
	
	matrix_ref<T, Window<Plain>> window(window_spec spec) const {
		return matrix_ref<T, Window<Plain>>(*this, spec);
	}
	
	matrix_ref<T, Diagonal<Plain>> diagonal() const {
		return matrix_ref<T, Diagonal<Plain>>(*this);
	}
	
	const matrix_ref<T, Diagonal_matrix<Plain>> diagonal_matrix() const {
		return matrix_ref<T, Diagonal_matrix<Plain>>(*this);
	}
	
	unsigned get_height() const { return height; }
	unsigned get_width() const { return width; }
	
	
	protected:
	matrix_ref(){}
		
	std::shared_ptr<std::vector<T>> data;
	unsigned height, width;

};


template<typename T, class decorated> 
class matrix_ref<T, Transpose<decorated>> : private matrix_ref<T, decorated> {
	public:
	
	//type members
	typedef T type;
	typedef Transpose<decorated> matrix_type;
	typedef matrix_ref<T, decorated> base;
	friend class matrix_ref<T, decorated>;
	using base::iterator;
	using base::const_iterator;
	typedef typename base::row_iterator col_iterator;
	typedef typename base::const_row_iterator const_col_iterator;
	typedef typename base::col_iterator row_iterator;
	typedef typename base::const_col_iterator const_row_iterator;
	
	
	T& operator ()( unsigned row, unsigned column ) 
	{ return base::operator()(column, row); }
	const T& operator ()( unsigned row, unsigned column ) const
	{ return base::operator()(column, row); }
	
	using base::begin;
	using base::end;
	col_iterator col_begin(unsigned i) { return base::row_begin(i); }
	const_col_iterator col_begin(unsigned i) const { return base::row_begin(i); }
	row_iterator row_begin(unsigned i) { return base::col_begin(i); }
	const_row_iterator row_begin(unsigned i) const { return base::col_begin(i); }
	
	base transpose() const { return *this; }
	
	matrix_ref<T, Window<Transpose<decorated>>> window(window_spec spec) const {
		return matrix_ref<T, Window<Transpose<decorated>>>(*this, spec);
	}
	
	matrix_ref<T, Diagonal<Transpose<decorated>>> diagonal() const {
		return matrix_ref<T, Diagonal<Transpose<decorated>>>(*this);
	}
	
	const matrix_ref<T, Diagonal_matrix<Transpose<decorated>>> diagonal_matrix() const {
		return matrix_ref<T, Diagonal_matrix<Transpose<decorated>>>(*this);
	}
	
	unsigned get_height() const { return base::get_width(); }
	unsigned get_width() const { return base::get_height(); }
		
	private:
	matrix_ref(const base&X) : base(X) {}
};



template<typename T, class decorated> 
class matrix_ref<T, Window<decorated>> : private matrix_ref<T, decorated> {
	public:
	
	//type members
	typedef T type;
	typedef Window<decorated> matrix_type;
	typedef matrix_ref<T, decorated> base;
	friend class matrix_ref<T, decorated>;
	
	typedef index_row_iterator<T,Window<decorated>> iterator;
	typedef const_index_row_iterator<T,Window<decorated>> const_iterator;
	typedef index_row_iterator<T,Window<decorated>> row_iterator;
	typedef const_index_row_iterator<T,Window<decorated>> const_row_iterator;
	typedef index_col_iterator<T,Window<decorated>> col_iterator;
	typedef const_index_col_iterator<T,Window<decorated>> const_col_iterator;
	
	
	T& operator ()( unsigned row, unsigned column ) 
	{ return base::operator()(row+spec.row_start, column+spec.col_start); }
	const T& operator ()( unsigned row, unsigned column ) const
	{ return base::operator()(row+spec.row_start, column+spec.col_start); }
	
	
	iterator begin() { return iterator(*this,0,0); }
	iterator end() { return iterator(*this,get_height(),0); }
	const_iterator begin() const { return const_iterator(*this,0,0); }
	const_iterator end() const { return const_iterator(*this,get_height(),0); }
	
	row_iterator row_begin(unsigned i) { return row_iterator(*this,i,0); }
	row_iterator row_end(unsigned i) { return row_iterator(*this,i+1,0); }
	const_row_iterator row_begin(unsigned i) const { return const_row_iterator(*this,i,0); }
	const_row_iterator row_end(unsigned i) const { return const_row_iterator(*this,i+1,0); }
	
	col_iterator col_begin(unsigned i) { return col_iterator(*this,0,i); }
	col_iterator col_end(unsigned i) { return col_iterator(*this,0,i+1); }
	const_col_iterator col_begin(unsigned i) const { return const_col_iterator(*this,0,i); }
	const_col_iterator col_end(unsigned i) const { return const_col_iterator(*this,0,i+1); }
	
	
	matrix_ref<T, Transpose<Window<decorated>>> transpose() const { 
		return matrix_ref<T, Transpose<Window<decorated>>>(*this);
	}
	
	matrix_ref<T, Window<decorated>> window(window_spec win) const {
		return matrix_ref<T, Window<decorated>>(*this, {
			spec.row_start+win.row_start,
			spec.row_start+win.row_end,
			spec.col_start+win.col_start,
			spec.col_start+win.col_end});
	}
	
	matrix_ref<T, Diagonal<Window<decorated>>> diagonal() const {
		return matrix_ref<T, Diagonal<Window<decorated>>>(*this);
	}
	
	const matrix_ref<T, Diagonal_matrix<Window<decorated>>> diagonal_matrix() const {
		return matrix_ref<T, Diagonal_matrix<Window<decorated>>>(*this);
	}
	
	unsigned get_height() const { return spec.row_end-spec.row_start; }
	unsigned get_width() const { return spec.col_end-spec.col_start; }
	
	
	
		
	private:
	matrix_ref(const base&X, window_spec win) : base(X), spec(win) {
			assert(spec.row_end<=base::get_height());
			assert(spec.col_end<=base::get_width());
	}
	
	window_spec spec;
};


template<typename T, class decorated> 
class matrix_ref<T, Diagonal<decorated>> : private matrix_ref<T, decorated> {
	public:
	
	//type members
	typedef T type;
	typedef Diagonal<decorated> matrix_type;
	typedef matrix_ref<T, decorated> base;
	friend class matrix_ref<T, decorated>;
	
	typedef index_col_iterator<T,Diagonal<decorated>> iterator;
	typedef const_index_col_iterator<T,Diagonal<decorated>> const_iterator;
	typedef index_row_iterator<T,Diagonal<decorated>> row_iterator;
	typedef const_index_row_iterator<T,Diagonal<decorated>> const_row_iterator;
	typedef index_col_iterator<T,Diagonal<decorated>> col_iterator;
	typedef const_index_col_iterator<T,Diagonal<decorated>> const_col_iterator;
	
	
	T& operator ()( unsigned row, unsigned column=0 ) 
	{
		assert(column==0);
		return base::operator()(row,row);
	}
	const T& operator ()( unsigned row, unsigned column=0 ) const
	{
		assert(column==0);
		return base::operator()(row,row);
	}
	
	
	iterator begin() { return iterator(*this,0,0); }
	iterator end() { return iterator(*this,0,1); }
	const_iterator begin() const { return const_iterator(*this,0,0); }
	const_iterator end() const { return const_iterator(*this,0,1); }
	
	row_iterator row_begin(unsigned i) { return row_iterator(*this,i,0); }
	row_iterator row_end(unsigned i) { return row_iterator(*this,i+1,0); }
	const_row_iterator row_begin(unsigned i) const { return const_row_iterator(*this,i,0); }
	const_row_iterator row_end(unsigned i) const { return const_row_iterator(*this,i+1,0); }
	
	col_iterator col_begin(unsigned i) { return col_iterator(*this,0,i); }
	col_iterator col_end(unsigned i) { return col_iterator(*this,0,i+1); }
	const_col_iterator col_begin(unsigned i) const { return const_col_iterator(*this,0,i); }
	const_col_iterator col_end(unsigned i) const { return const_col_iterator(*this,0,i+1); }
	
	
	
	matrix_ref<T, Transpose<Diagonal<decorated>>> transpose() const { 
		return matrix_ref<T, Transpose<Diagonal<decorated>>>(*this);
	}
	
	matrix_ref<T, Window<Diagonal<decorated>>> window(window_spec win) const {
		return matrix_ref<T, Window<Diagonal<decorated>>>(*this, win);
	}
	
	matrix_ref<T, Diagonal<Diagonal<decorated>>> diagonal() const {
		return matrix_ref<T, Diagonal<Diagonal<decorated>>>(*this);
	}
	
	const matrix_ref<T, Diagonal_matrix<Diagonal<decorated>>> diagonal_matrix() const {
		return matrix_ref<T, Diagonal_matrix<Diagonal<decorated>>>(*this);
	}
	
	unsigned get_height() const { 
		return std::min(base::get_height(), base::get_width()); 
		}
	unsigned get_width() const { return 1; }
		
	private:
	matrix_ref(const base&X) : base(X) {}
};



template<typename T, class decorated> 
class matrix_ref<T, Diagonal_matrix<decorated>> : private matrix_ref<T, decorated> {
	public:
	
	//type members
	typedef T type;
	typedef Diagonal_matrix<decorated> matrix_type;
	typedef matrix_ref<T, decorated> base;
	friend class matrix_ref<T, decorated>;
	
	//typedef index_row_iterator<T,Diagonal_matrix<decorated>> iterator;
	typedef const_index_row_iterator<T,Diagonal_matrix<decorated>> const_iterator;
	//typedef index_row_iterator<T,Diagonal_matrix<decorated>> row_iterator;
	typedef const_index_row_iterator<T,Diagonal_matrix<decorated>> const_row_iterator;
	//typedef index_col_iterator<T,Diagonal_matrix<decorated>> col_iterator;
	typedef const_index_col_iterator<T,Diagonal_matrix<decorated>> const_col_iterator;
	
	//Diagonal_matrix is always const!
	/*
	T& operator ()( unsigned row, unsigned column ) 
	{
		if (row!=column) return zero;
		else return base::operator()(row,row);
	}
	*/
	const T& operator ()( unsigned row, unsigned column) const
	{
		if (row!=column) return zero;
		else return base::operator()(row,0);
	}
	
	
	//iterator begin() { return iterator(*this,0,0); }
	//iterator end() { return iterator(*this,get_height(),0); }
	const_iterator begin() const { return const_iterator(*this,0,0); }
	const_iterator end() const { return const_iterator(*this,get_height(),0); }
	
	//row_iterator row_begin(unsigned i) { return row_iterator(*this,i,0); }
	//row_iterator row_end(unsigned i) { return row_iterator(*this,i+1,0); }
	const_row_iterator row_begin(unsigned i) const { return const_row_iterator(*this,i,0); }
	const_row_iterator row_end(unsigned i) const { return const_row_iterator(*this,i+1,0); }
	
	//col_iterator col_begin(unsigned i) { return col_iterator(*this,0,i); }
	//col_iterator col_end(unsigned i) { return col_iterator(*this,0,i+1); }
	const_col_iterator col_begin(unsigned i) const { return const_col_iterator(*this,0,i); }
	const_col_iterator col_end(unsigned i) const { return const_col_iterator(*this,0,i+1); }
	
	
	matrix_ref<T, Transpose<Diagonal_matrix<decorated>>> transpose() const { 
		return matrix_ref<T, Transpose<Diagonal_matrix<decorated>>>(*this);
	}
	
	matrix_ref<T, Window<Diagonal_matrix<decorated>>> window(window_spec win) const {
		return matrix_ref<T, Window<Diagonal_matrix<decorated>>>(*this, win);
	}
	
	matrix_ref<T, decorated> diagonal() const { 
		return matrix_ref<T, decorated>(*this);
	}
	
	const matrix_ref<T, Diagonal_matrix<decorated>> diagonal_matrix() const {
		assert(false);
		return *this;
	}
	
	
	
	unsigned get_height() const { return base::get_height(); }
	unsigned get_width() const { return base::get_height(); }
		
	private:
	matrix_ref(const base&X) : base(X), zero(0) { assert(base::get_width()==1); }
	const T zero;
};






template<typename T> 
class matrix : public matrix_ref<T,Plain> {
	public:
	
	matrix( unsigned height, unsigned width ) {
		this->height = height;
		this->width = width;
		data = std::make_shared<std::vector<T>>(width*height);
	}
	
	matrix(const matrix<T>&X) {
		height = X.height;
		width = X.width;
		data = std::make_shared<std::vector<T>>(width*height);
		*data = *(X.data);
	}
	
	template<class matrix_type>
	matrix(const matrix_ref<T,matrix_type>&X) {
		height = X.get_height();
		width = X.get_width();
		data = std::make_shared<std::vector<T>>(width*height);
		//copy does not work as my row_iterators do not provide all the facilities of iterators
		//std::copy(X.row_begin(0),X.row_begin(height),data->begin());
		auto source=X.row_begin(0);
		const auto end=X.row_begin(height);
		auto dest=data->begin();
		while (source!=end) {
			*dest = *source;
			++dest;
			++source;
		}
	}
	

	private:
	using matrix_ref<T,Plain>::height;
	using matrix_ref<T,Plain>::width; 
	using matrix_ref<T,Plain>::data; 

};

#endif //_MATRIX_H_
