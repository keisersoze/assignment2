#ifndef _MATRIX_H_
#define _MATRIX_H_

#include<vector>
#include<memory>
#include<cassert>

#include"matrix_fwd.h"
#include"iterators.h"




template<typename T, unsigned R, unsigned C>
class matrix_ref<T, Plain<R,C>> {
	public:
	
	//type members
	typedef T type;
	typedef Plain<R,C> matrix_type;
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;
	typedef typename std::vector<T>::iterator row_iterator;
	typedef typename std::vector<T>::const_iterator const_row_iterator;
	
	typedef index_col_iterator<T,Plain <R,C>> col_iterator;
	typedef const_index_col_iterator<T,Plain <R,C>> const_col_iterator;
	
	
	T& operator ()( unsigned row, unsigned column ) { 
		return data->operator[](row*C + column);
	}
	const T& operator ()( unsigned row, unsigned column ) const { 
		return data->operator[](row*C + column);
	}
	
	
	
	iterator begin() { return data->begin(); }
	iterator end() { return data->end(); }
	const_iterator begin() const { return data->begin(); }
	const_iterator end() const { return data->end(); }
	
	row_iterator row_begin(unsigned i) { return data->begin() + i*C; }
	row_iterator row_end(unsigned i) { return data->begin() + (i+1)*C; }
	const_row_iterator row_begin(unsigned i) const { return data->begin() + i*C; }
	const_row_iterator row_end(unsigned i) const { return data->begin() + (i+1)*C; }
	
	col_iterator col_begin(unsigned i) { return col_iterator(*this,0,i); }
	col_iterator col_end(unsigned i) { return col_iterator(*this,0,i+1); }
	const_col_iterator col_begin(unsigned i) const { return const_col_iterator(*this,0,i); }
	const_col_iterator col_end(unsigned i) const { return const_col_iterator(*this,0,i+1); }
	
	
	matrix_ref<T, Transpose<Plain<R,C>>> transpose() const {
		return matrix_ref<T, Transpose<Plain<R,C>>>(*this);
	}

	template <unsigned R2, unsigned  C2>
	matrix_ref<T, Window<Plain<R,C>,R2,C2>> window(window_spec spec) const {
		return matrix_ref<T, Window<Plain<R,C>,R2,C2>>(*this, spec);
	}
	
	matrix_ref<T, Diagonal<Plain<R,C>>> diagonal() const {
		return matrix_ref<T, Diagonal<Plain<R,C>>>(*this);
	}
	
	const matrix_ref<T, Diagonal_matrix<Plain<R,C>>> diagonal_matrix() const {
		return matrix_ref<T, Diagonal_matrix<Plain<R,C>>>(*this);
	}

	constexpr unsigned get_height() const { return R; }
	constexpr unsigned get_width() const { return C; }
	
	
	protected:
	matrix_ref(){}
		
	std::shared_ptr<std::array<T, R*C>> data;

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

    template <unsigned R2, unsigned  C2>
	matrix_ref<T, Window<Transpose<decorated>,R2,C2>> window(window_spec spec) const {
		return matrix_ref<T, Window<Transpose<decorated>,R2,C2>>(*this, spec);
	}
	
	matrix_ref<T, Diagonal<Transpose<decorated>>> diagonal() const {
		return matrix_ref<T, Diagonal<Transpose<decorated>>>(*this);
	}
	
	const matrix_ref<T, Diagonal_matrix<Transpose<decorated>>> diagonal_matrix() const {
		return matrix_ref<T, Diagonal_matrix<Transpose<decorated>>>(*this);
	}
	
	constexpr unsigned get_height() const { return base::get_width(); }
	constexpr unsigned get_width() const { return base::get_height(); }
		
	private:
	matrix_ref(const base&X) : base(X) {}
};



template<typename T, class decorated, unsigned R, unsigned C>
class matrix_ref<T, Window<decorated, R, C>> : private matrix_ref<T, decorated> {
	public:
	
	//type members
	typedef T type;
	typedef Window<decorated,R,C> matrix_type;
	typedef matrix_ref<T, decorated> base;
	friend class matrix_ref<T, decorated>;
	
	typedef index_row_iterator<T,Window<decorated,R,C>> iterator;
	typedef const_index_row_iterator<T,Window<decorated,R,C>> const_iterator;
	typedef index_row_iterator<T,Window<decorated,R,C>> row_iterator;
	typedef const_index_row_iterator<T,Window<decorated,R,C>> const_row_iterator;
	typedef index_col_iterator<T,Window<decorated,R,C>> col_iterator;
	typedef const_index_col_iterator<T,Window<decorated,R,C>> const_col_iterator;
	
	
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
	
	
	matrix_ref<T, Transpose<Window<decorated,R,C>>> transpose() const {
		return matrix_ref<T, Transpose<Window<decorated,R,C>>>(*this);
	}
	
	matrix_ref<T, Window<decorated,R,C>> window(window_spec win) const {
		return matrix_ref<T, Window<decorated,R,C>>(*this, {
			spec.row_start+win.row_start,
			spec.row_start+win.row_end,
			spec.col_start+win.col_start,
			spec.col_start+win.col_end});
	}
	
	matrix_ref<T, Diagonal<Window<decorated,R,C>>> diagonal() const {
		return matrix_ref<T, Diagonal<Window<decorated,R,C>>>(*this);
	}
	
	const matrix_ref<T, Diagonal_matrix<Window<decorated,R,C>>> diagonal_matrix() const {
		return matrix_ref<T, Diagonal_matrix<Window<decorated,R,C>>>(*this);
	}

	constexpr unsigned get_height() const { return R; }
	constexpr unsigned get_width() const { return C; }
	
	
	
		
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

	template <unsigned R2,unsigned C2>
	matrix_ref<T, Window<Diagonal<decorated>,R2,C2>> window(window_spec win) const {
		return matrix_ref<T, Window<Diagonal<decorated>,R2,C2>>(*this, win);
	}
	
	matrix_ref<T, Diagonal<Diagonal<decorated>>> diagonal() const {
		return matrix_ref<T, Diagonal<Diagonal<decorated>>>(*this);
	}
	
	const matrix_ref<T, Diagonal_matrix<Diagonal<decorated>>> diagonal_matrix() const {
		return matrix_ref<T, Diagonal_matrix<Diagonal<decorated>>>(*this);
	}

	constexpr unsigned get_height() const {
		return std::min(base::get_height(), base::get_width()); 
		}
	constexpr unsigned get_width() const { return 1; }
		
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

	template<unsigned R2, unsigned C2>
	matrix_ref<T, Window<Diagonal_matrix<decorated>,R2,C2>> window(window_spec win) const {
		return matrix_ref<T, Window<Diagonal_matrix<decorated>,R2,C2>>(*this, win);
	}
	
	matrix_ref<T, decorated> diagonal() const { 
		return matrix_ref<T, decorated>(*this);
	}
	
	const matrix_ref<T, Diagonal_matrix<decorated>> diagonal_matrix() const {
		assert(false);
		return *this;
	}



	constexpr unsigned get_height() const { return base::get_height(); }
	constexpr unsigned get_width() const { return base::get_height(); }
		
	private:
	matrix_ref(const base&X) : base(X), zero(0) { assert(base::get_width()==1); }
	const T zero;
};






template<typename T, unsigned R, unsigned C>
class matrix : public matrix_ref<T,Plain<R,C>> {
	public:
	
	matrix( ) {
		data = std::make_shared<std::array<T, R*C>>();
	}
	
	matrix(const matrix<T, R, C> &X) {
		data = std::make_shared<std::array<T, R*C>>();
		*data = *(X.data);
	}
	
	template<class matrix_type>
	matrix(const matrix_ref<T,matrix_type>&X) {
		data = std::make_shared<std::array<T, R*C>>();
		//copy does not work as my row_iterators do not provide all the facilities of iterators
		//std::copy(X.row_begin(0),X.row_begin(height),data->begin());
		auto source=X.row_begin(0);
		const auto end=X.row_begin(R);
		auto dest=data->begin();
		while (source!=end) {
			*dest = *source;
			++dest;
			++source;
		}
	}

	private:
	using matrix_ref<T,Plain<R,C>>::data;

};

#endif //_MATRIX_H_
