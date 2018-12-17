#ifndef _MATRIX_H_
#define _MATRIX_H_

#include<vector>
#include<memory>
#include<cassert>

#include"matrix_fwd.h"
#include"iterators.h"

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

	//dummy static dimensions, used by the static matrix variant to check dimension
	//at compile time using the "foreign matrix" constructor (0 dimension is not 
	//allowed by the static matrix, if foreign matrix is not static then the 
	//dimension consistency is checked at run time)
	static constexpr unsigned h = 0;
	static constexpr unsigned w = 0;
	
	T& operator ()( unsigned row, unsigned column ) {
		return data->operator[](row*width + column);
	}
	const T& operator ()( unsigned row, unsigned column ) const { 
		return data->operator[](row*width + column);
	}

	// static assertion cannot be done
	template<unsigned i, unsigned j>
	T& get() {
		return operator()(i, j);
	}

	// static assertion cannot be done
	template<unsigned i, unsigned j>
	const T& get() const {
		return operator()(i, j);
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


template<typename T, unsigned H, unsigned W> 
class matrix_ref<T, CT_dims<H,W>> {
	public:
	
	//type members
	typedef T type;
	typedef CT_dims<H,W> matrix_type;
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;
	typedef typename std::vector<T>::iterator row_iterator;
	typedef typename std::vector<T>::const_iterator const_row_iterator;
	
	typedef index_col_iterator<T,CT_dims<H,W>> col_iterator;
	typedef const_index_col_iterator<T,CT_dims<H,W>> const_col_iterator;

	//static dimensions
	static constexpr unsigned h = H;
	static constexpr unsigned w = W;
	
	T& operator ()( unsigned row, unsigned column ) {
		return data->operator[](row*width + column);
	}
	const T& operator ()( unsigned row, unsigned column ) const { 
		return data->operator[](row*width + column);
	}

	template<unsigned i, unsigned j>
	T& get() {
		static_assert( i >= 0 && i < h && j >= 0 && j < w, "index out of bound" );
		return operator()(i, j);
	}

	template<unsigned i, unsigned j>
	const T& get() const {
		static_assert( i >= 0 && i < h && j >= 0 && j < w, "index out of bound" );
		return operator()(i, j);
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
	
	
	matrix_ref<T, Transpose<CT_dims<H,W>>> transpose() const { 
		return matrix_ref<T, Transpose<CT_dims<H,W>>>(*this);
	}
	
	matrix_ref<T, Window<CT_dims<H,W>>> window(window_spec spec) const {
		return matrix_ref<T, Window<CT_dims<H,W>>>(*this, spec);
	}
	
	matrix_ref<T, Diagonal<CT_dims<H,W>>> diagonal() const {
		return matrix_ref<T, Diagonal<CT_dims<H,W>>>(*this);
	}
	
	const matrix_ref<T, Diagonal_matrix<CT_dims<H,W>>> diagonal_matrix() const {
		return matrix_ref<T, Diagonal_matrix<CT_dims<H,W>>>(*this);
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

	static constexpr unsigned h = base::w;
	static constexpr unsigned w = base::h;
	
	
	T& operator ()( unsigned row, unsigned column ) 
	{ return base::operator()(column, row); }
	const T& operator ()( unsigned row, unsigned column ) const
	{ return base::operator()(column, row); }

	// no need to check both heigh and width to be nonzero, they are either both 0 or >0
	template<unsigned i, unsigned j>
	T& get() {
		static_assert( h != 0 && i < h && j < w, "index out of bound" );
		return base::template get<j,i>();
	}

	template<unsigned i, unsigned j>
	const T& get() const {
		static_assert( h != 0 && i < h && j < w, "index out of bound" );
		return base::template get<j,i>();
	}
	
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
	
	//no static window allowed, decorated matrix size known only at runtime
	static constexpr unsigned h = 0;
	static constexpr unsigned w = 0;
	
	T& operator ()( unsigned row, unsigned column ) 
	{ return base::operator()(row+spec.row_start, column+spec.col_start); }
	const T& operator ()( unsigned row, unsigned column ) const
	{ return base::operator()(row+spec.row_start, column+spec.col_start); }
	
	// static assertion cannot be done
	template<unsigned i, unsigned j>
	T& get() {
		return operator()(i, j);
	}

	// static assertion cannot be done
	template<unsigned i, unsigned j>
	const T& get() const {
		return operator()(i, j);
	}

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
	
	static constexpr unsigned h = base::h;
	static constexpr unsigned w = (base::w == 0) ? 0 : 1; // >0 if CT_dims is base class
	
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

	template<unsigned i, unsigned j=0>
	T& get() {
		static_assert( h != 0 && i >= 0 && i < h && j == 0, "index out of bound" );
		return base::template get<i,j>();
	}
	
	template<unsigned i, unsigned j=0>
	const T& get() const {
		static_assert( h != 0 && i >= 0 && i < h && j == 0, "index out of bound" );
		return base::template get<i,j>();
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

	static constexpr unsigned h = base::h;
	static constexpr unsigned w = base::h;
	
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

	//Diagonal_matrix is always const!
	/*
	template<unsigned i, unsigned j>
	T& get() {
		static_assert( h != 0 && i >= 0 && i < h && j >= 0 && j < w, "index out of bound" );
		if ( i != j ) return zero;
		else return base::template get<i,i>();
	}
	*/

	template<unsigned i, unsigned j>
	const T& get() const {
		static_assert( h != 0 && i >= 0 && i < h && j >= 0 && j < w, "index out of bound" );
		if ( i != j ) return zero;
		else return base::template get<i,0>();
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


// auxiliary templated type for matrix class specialization
// ellipsis allows empty variadic type list to preserve original type
template<typename T, unsigned...dims> class matrix;


template<typename T> 
class matrix<T> : public matrix_ref<T,Plain> {
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

template<typename T, unsigned H, unsigned W> 
class matrix<T,H,W> : public matrix_ref<T,CT_dims<H,W>> {
	public:

	static_assert( H > 0 && W > 0, "matrix cannot have 0 or negative dimension" );
	
	matrix( void ) {
		this->height = H;
		this->width = W;
		data = std::make_shared<std::vector<T>>(width*height);
	}
	
	matrix(const matrix<T,H,W>&X) {
		height = X.height;
		width = X.width;
		data = std::make_shared<std::vector<T>>(width*height);
		*data = *(X.data);
	}
	
	template<class matrix_type>
	matrix(const matrix_ref<T,matrix_type>&X) {
		static_assert( (matrix_ref<T,matrix_type>::h == 0 || matrix_ref<T,matrix_type>::h == H) &&
					   (matrix_ref<T,matrix_type>::w == 0 || matrix_ref<T,matrix_type>::h == H), 
					   "matrix size mismatch in static sized matrix constructor" );

		height = X.get_height();
		width = X.get_width();
		assert( height == H && width == W );
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
	
	using matrix_ref<T,CT_dims<H,W>>::h;
	using matrix_ref<T,CT_dims<H,W>>::w;

	private:
	using matrix_ref<T,CT_dims<H,W>>::height;
	using matrix_ref<T,CT_dims<H,W>>::width; 
	using matrix_ref<T,CT_dims<H,W>>::data; 

};

#endif //_MATRIX_H_
