// Jonathon Hamrick
// -Personal implementation of a vector<> type
// 10/1/2012
// 10/3/2012

#ifndef JH_VECTOR
#define JH_VECTOR

template <typename Type = int>
class jhVector
{
public:
	jhVector();							// default constructor
	jhVector(unsigned int uiReserve);	// overloaded constructor to reserver a specific amount of space

	/* Trilogy of Evil */
	jhVector(const jhVector& cpy);
	~jhVector();
	jhVector& operator=(const jhVector& cpy);

	/* Accessors */
	unsigned int Size() const { return uiSize; }			// return the current size of the jhVector
	unsigned int Capacity() const { return uiCapacity; }	// return the current capacity of the jhVector

	/* Operators */
	Type& operator[](int iIndex) { return ptArray[iIndex]; }				// access a specific item in the vector and be able to change it (does not error check)
	const Type& operator[](int iIndex) const { return ptArray[iIndex]; }	// access a specific item in the vector only for viewing (does not error check)

	/* Methods */
	void Append(const Type& tData);				// add an item to the vector
	bool Reserve(unsigned int uiReserve = 0);	// expand the vector to the specified amount in uiReserve (default expansion is double uiCapacity if uiReserve is 0)
	void Clear();								// clear the existing data in the vector
	void Remove();								// "remove" the last guy from the vector (error checking only makes sure something is in the vector)
	void Remove(int iIndex);					// "remove" at target location and move all other items forward in the vector (error checking only makes sure something is in the vector)

	void Merge(const jhVector& mergVec, bool dup = false);				// merge a vector to the invoking vector, default behavior to not retain duplicates
protected:
	/* Data Members */
	Type* ptArray;						// the backend array for the class
	unsigned int uiSize, uiCapacity;	// the size(actual amount) and capacity(possible amount) of the jhVector

private:
};

template <typename Type>
jhVector<Type>::jhVector()
{
	ptArray = nullptr;
	uiSize = uiCapacity = 0;
}

template <typename Type>
jhVector<Type>::jhVector(unsigned int uiReserve)
{
	ptArray = nullptr;
	uiSize = uiCapacity = 0;

	if (uiReserve > 0)
	{
		ptArray = new Type[uiReserve];
		uiCapacity = uiReserve;
	}
}

template <typename Type>
jhVector<Type>::jhVector(const jhVector& cpy)
{
	ptArray = new Type[cpy.uiCapacity];
	uiSize = cpy.uiSize;
	uiCapacity = cpy.uiCapacity;

	for (unsigned int i = 0; i < uiSize; ++i)
		ptArray[i] = cpy[i];
}

template <typename Type>
jhVector<Type>::~jhVector()
{
	delete [] ptArray;
}

template <typename Type>
jhVector<Type>& jhVector<Type>::operator=(const jhVector& cpy)
{
	if (this == &cpy)
		return *this;

	delete [] ptArray;
	ptArray = new Type[cpy.uiCapacity];
	uiSize = cpy.uiSize;
	uiCapacity = cpy.uiCapacity;

	for (unsigned int i = 0; i < uiSize; ++i)
		ptArray[i] = cpy[i];

	return *this;
}

template <typename Type>
void jhVector<Type>::Append(const Type& tData)
{
	if (uiSize == uiCapacity)
		Reserve();

	ptArray[uiSize++] = tData;
}

template <typename Type>
bool jhVector<Type>::Reserve(unsigned int uiReserve)
{
	if (uiReserve == 0)
	{
		if (uiCapacity == 0)
			uiCapacity = 1;
		else
			uiCapacity <<= 1;
	}
	else
	{
		if (uiCapacity < uiReserve)
			uiCapacity = uiReserve;
		else
			return false;
	}

	Type* tTemp = ptArray;
	ptArray = new Type[uiCapacity];

	for (unsigned int i = 0; i < uiSize; ++i)
		ptArray[i] = tTemp[i];

	delete [] tTemp;

	return true;
}

template <typename Type>
void jhVector<Type>::Clear()
{
	delete [] ptArray;
	ptArray = nullptr;
	uiSize = uiCapacity = 0;
}

template <typename Type>
void jhVector<Type>::Remove()
{
	if (uiSize > 0)
		uiSize--;
}

template <typename Type>
void jhVector<Type>::Remove(int iIndex)
{
	if (uiSize > 0)
	{
		uiSize--;
		for (unsigned int i = iIndex; i < uiSize; ++i)
		{
			ptArray[i] = ptArray[i + 1];
		}

		
	}
}

template <typename Type>
void jhVector<Type>::Merge(const jhVector& mergVec, bool dup)
{
	// add the contents of the incoming vector to the invoking vector
	for (unsigned int i = 0; i < mergVec.Size(); ++i)
		Append(mergVec[i]);

	// remove duplicates
	if (dup == false)
	{
		for (unsigned int i = 0; i < Size(); ++i)
		{
			for (unsigned int j = 0; j < Size(); ++j)
			{
				if (i == j)
					continue;

				if (ptArray[i] == ptArray[j])
				{
					Remove(j--);
				}
			}
		}
	}
}
#endif