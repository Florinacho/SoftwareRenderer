#ifndef __FIXED_DOUBLE_ENDED_QUEUE_H__
#define __FIXED_DOUBLE_ENDED_QUEUE_H__

#define __ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define fdeq(T, S) struct {\
	T data[S];\
	unsigned int begin;\
	unsigned int count;\
}

#define fdeq_init(this) {\
	(this)->begin = 0;\
	(this)->count = 0;\
}

#define fdeq_clear(this) {\
	(this)->begin = 0;\
	(this)->count = 0;\
}

#define fdeq_push_front(this, value) {\
	const unsigned int _SIZE = __ARRAY_SIZE((this)->data);\
	(this)->count += ((this)->count < _SIZE);\
	(this)->begin  = (((this)->begin == 0) ? (_SIZE - 1) : ((this)->begin - 1));\
	(this)->data[(this)->begin] = (value);\
}

#define fdeq_push_back(this, value) {\
	const unsigned int _SIZE = __ARRAY_SIZE((this)->data);\
	(this)->data[((this)->begin + (this)->count) % _SIZE] = (value);\
	(this)->begin  = ((this)->begin + ((this)->count + 1 > _SIZE)) % _SIZE;\
	(this)->count += ((this)->count < _SIZE);\
}

#define fdeq_set(this, index, value)  {(this)->data[((this)->begin + (index)) % __ARRAY_SIZE((this)->data)] = (value);}

#define fdeq_get(this, index)         ((this)->data[((this)->begin + (index)) % __ARRAY_SIZE((this)->data)])

#define fdeq_size(this)               ((const unsigned int)(this)->count)

#define fdeq_pop_front(this, value)   (((this)->count > 0) ? ({ *(value) = (this)->data[(this)->begin]; (this)->begin = ((this)->begin + 1) % __ARRAY_SIZE((this)->data); (this)->count -= 1; }), 1 : 0)

#define fdeq_pop_back(this, value)    (((this)->count > 0) ? ({ (this)->count -= 1; *(value) = (this)->data[((this)->begin + (this)->count) % __ARRAY_SIZE((this)->data)]; }), 1 : 0)

#endif // __FIXED_DOUBLE_ENDED_QUEUE_H__
