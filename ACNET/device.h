// $Id: device.h,v 1.5 2011/11/10 22:38:15 jerryf Exp $

#ifndef __DEVICE_H
#define __DEVICE_H

#include <stdexcept>
#include <vector>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "acnetex.h"

// These #defines are for the ReqInfo event_type field.
#define EV_FTD 0
#define EV_PERIODIC 1
#define EV_CLOCK 2
#define EV_STATE 3
#define EV_USPERIOD 4
#define EV_OTHER 5

namespace FFF {

    // This class is the same size as a 32-bit int, but it carries
    // with it the type baggage that is length-specific.

#if !defined(NDEBUG) || defined(TYPE_SAFETY)
    class Count {
	friend class Index;
	uint32_t cnt;

     public:
	explicit Count(uint32_t _l) : cnt(_l) {}
	Count operator+(Count const& o) const { return Count(cnt + o.cnt); } // XXX: check overflow
	Count operator-(Count const& o) const { return Count(cnt - o.cnt); } // XXX: check underflow
	bool operator<(Count const& o) const { return cnt < o.cnt; }
	bool operator==(Count const& o) const { return cnt == o.cnt; }
	uint32_t getValue() const { return cnt; }

	Count operator/(size_t const n) const
	{

	    //fprintf(stderr, "device.h: count %d size=%d\n", cnt, n);
	    if (cnt % n == 0)
		return Count(cnt / n);
	    else{
	      fprintf(stderr, "count %d is not multiple of data size %d\n", cnt, (int) n);
		throw Ex_BADLEN; //std::runtime_error("count is not multiple of data size");
	    }
	}
    };
#else
    typedef uint32_t Count;
#endif

    // This class enforces "offset-ness". It doesn't allow two offsets
    // to be added together, but it does allow a length to be added to
    // it.

#if !defined(NDEBUG) || defined(TYPE_SAFETY)
    class Index {
	int32_t idx;

     public:
	explicit Index(uint32_t _o) : idx(_o) {}
	Index operator+(Count const& o) const { return Index(idx + o.cnt); } // XXX: check overflow
	Index operator-(Count const& o) const { return Index(idx - o.cnt); }	// XXX: check underflow
	Count operator-(Index const& o) const { return Count(idx - o.idx); }	// XXX: check underflow
	Index operator++() { return Index(idx++); }
	Index& operator++(int) { ++idx; return *this; }
	Index operator--() { return Index(idx--); }
	Index& operator--(int) { --idx; return *this; }
	bool operator==(Index const& o) const { return idx == o.idx; }
	bool operator<(Index const& o) const { return idx < o.idx; }
	bool operator<=(Index const& o) const { return idx <= o.idx; }
	int32_t getValue() const { return idx; }

	Index operator/(size_t const n) const
	{
	    if (idx % n == 0)
		return Index(idx / n);
	    else{
		fprintf(stderr, "index is not multiple of data size\n");
		throw Ex_BADLEN;//std::runtime_error("index is not multiple of data size");
	    }
	}
    };
#else
    typedef int32_t Index;
#endif

    class ReqInfo {
     protected:
	uint32_t eventType, eventVal;
	uint32_t megasec, seconds, microsec;
	uint32_t omsp;
	uint32_t requestid;
	uint32_t attr;

     public:
	ReqInfo(uint32_t _evType, uint32_t _evVal, uint32_t _mega,
		uint32_t _sec, uint32_t _micro) :
	    eventType(_evType), eventVal (_evVal), megasec(_mega),
	    seconds(_sec), microsec(_micro)
	{}

	ReqInfo() {}

	void setTime(uint32_t _evType, uint32_t _evVal, uint32_t _mega,
		     uint32_t _sec, uint32_t _micro)
	{
	    eventType = _evType;
	    eventVal  = _evVal;
	    megasec = _mega;
	    seconds = _sec;
	    microsec = _micro;
	}

	void setOMSP(uint32_t _omsp)
	{
	    omsp = _omsp;
	}

	void setAttr(uint32_t _attr)
	{
	    attr = _attr;
	}

	void set_requestid(uint32_t _id)
	{
	    requestid = _id;
	}

	void get_timespec(struct timespec* const when) const
	{
	    when->tv_sec = megasec * 1000000 + seconds;
	    when->tv_nsec = microsec * 1000;
	}

	uint32_t get_omsp() const
	{
	    return omsp;
	}

	uint32_t get_attr() const
	{
	    return attr;
	}

	uint32_t get_requestid() const
	{
	    return requestid;
	}

	void get_evTypeVal(uint32_t* const evType, uint32_t* const evVal) const
	{
	    *evType = eventType;
	    *evVal = eventVal;
	}
    };

    template <typename T>
    T const& deref(T const* const ptr, Index const ii)
    {
	return ptr[ii.getValue()];
    }

    template <typename T>
    T& deref(T* const ptr, Index const ii)
    {
	return ptr[ii.getValue()];
    }

    // This class is an "array proxy" in that it provides a view into
    // a raw buffer, but it does range checking.

    template <class _Type>
    class Array {
	_Type* const data;

	Array();
	Array(Array const&);
	Array& operator=(Array const& o);

     public:
	Index const offset;
	Count const total;

	Array(_Type _d[], Index _o, Count _n) :
	    data(_d), offset(_o), total(_n) {}

	_Type const& operator[](Index const ii) const
	{
	    if (inRange(ii))
		return deref(data, ii);
	    else{
 		//fprintf(stderr, "range_error: array ndx %d > Index(0) + maxndx %d\r\n",
		//ii.getValue(), total.getValue());
		throw Ex_BADOFLEN; //std::range_error("const Array<> : out of range index");
	    }
	}

	_Type& operator[](Index const ii)
	{
	    if (inRange(ii))
		return deref(data, ii);
	    else{
 		//fprintf(stderr, "range_error: array ndx %d > Index(0) + maxndx %d\r\n",
		//ii.getValue(), total.getValue());
		throw Ex_BADOFLEN; //std::range_error("Array<> : out of range index");
	    }
	}

	bool inRange(Index const ii) const { return ii - Index(0) < total; }

	inline _Type const& operator[](size_t const ii) const { return operator[](Index(ii)); }
	inline _Type& operator[](size_t const ii) { return operator[](Index(ii)); }
    };

    // This is the base support for a front-end device. The gist of it
    // is that a device is nothing more than a group of properties.
    // Rather than use the traditional ACNET view whereby a device
    // looks like a power supply (i.e. one reading, one setting, one
    // basic status, etc.), we let a device have as many properties as
    // needed. We can map these properties into the ACNET view. We
    // ought to also be able to map our devices to Channel Access
    // devices.

    class DeviceBase {

	// Cannot copy a device.

	DeviceBase();
	DeviceBase(DeviceBase const&);
	DeviceBase& operator=(DeviceBase const&);

     public:
	std::string const name;
	std::string const description;

	DeviceBase(std::string const& n, std::string const& d) :
	    name(n), description(d) {}
	virtual ~DeviceBase() {}

	virtual void getProperty(size_t, void*, Count, Index, ReqInfo const*) = 0;
	virtual void setProperty(size_t, void const*, Count, Index,ReqInfo const*) = 0;
	virtual void getPropertyFast(size_t, void*, Index, ReqInfo const*) = 0;
	virtual void initCollection(ReqInfo const*)  {}
	virtual void cleanupCollection(ReqInfo const*) {}
	virtual void startup_setting(char *,ReqInfo const*)  {}
    };

    template <unsigned total1>
    class Device : public DeviceBase {
	class ReadProxyBase;
	class WriteProxyBase;

     protected:
	template <class _Obj>
	struct MethodInfo {
	    std::string const name;
	    std::string const description;

	};

     private:

	// Abstract base class that simply defines the interface used
	// by any derived ReadProxy classes.

	class ReadProxyBase {
	 public:
	    virtual ~ReadProxyBase() {}

	    // standard method.

	    virtual void operator()(void*, Count, Index, ReqInfo const*) = 0;

	    // No length argument version (use maxLength as default)
	    // This is needed For FTP because FTP doesn't specify a
	    // length, the driver just returns as many points as it
	    // wants to.

	    virtual void operator()(void*, Index, ReqInfo const*) = 0;
	};

	// Templated class which defines specific instances of a
	// ReadProxy. This class turns a generic read which specifies
	// a buffer and length into a specific call to a method that
	// is strongly typed.

	template <class _Obj, class _Type>
	class ReadProxy : public ReadProxyBase {
	 public:
	    typedef void (_Obj::*Callback)(_Type&, ReqInfo const*);

	 private:
	    _Obj& ob;
	    Callback const cb;

	 public:
	    ReadProxy(_Obj& _ob, Callback _cb) : ob(_ob), cb(_cb) {}

	    void operator()(void* const buf, Count const len, Index const off, ReqInfo const* ri)
	    {
		//fprintf(stderr, "device.h: ReadProxy non-array len=%d\n", len.getValue());
		if (off == Index(0)) {
		    if (len == Count(sizeof(_Type)))
			(ob.*cb)(*reinterpret_cast<_Type*>(buf), ri);
		    else {
			fprintf(stderr,"ReadProxy bad length\n");
			throw Ex_BADLEN; //std::runtime_error("ReadProxy bad length");
		    }
		} else
		    throw Ex_BADOFF; //std::runtime_error("bad offset");
	    }

	    // Call the function with no length checking for non-Array
	    // types. I don't expect this next one to ever be called.
	    // It is only here to satisfy the virtual operator() that
	    // was added for the non-length-checking array usage
	    // (e.g. getPropertyFast)

	    void operator()(void* const buf, Index const off,
			    ReqInfo const* const ri)
	    {
	        //fprintf(stderr, "device.h: ReadProxy non-array len=%d\n", len.getValue());
		if (off == Index(0))
		    (ob.*cb)(*reinterpret_cast<_Type*>(buf), ri);
		else
		    throw Ex_BADOFF; //std::runtime_error("bad offset");
	    }
	};

	template <class _Obj, class _Type>
	class ReadProxy<_Obj, Array<_Type> > : public ReadProxyBase {
	 public:
	    typedef void (_Obj::*Callback)(Array<_Type>&,ReqInfo const*);

	 private:
	    _Obj& ob;
	    Callback const cb;
	    Index const maxIndex;

	 public:
	    ReadProxy(_Obj& _ob, Index const ms, Callback _cb) :
		ob(_ob), cb(_cb), maxIndex(ms)  {}

	    void operator()(void* const buf, Count const len, Index const off, ReqInfo const*ri)
	    {
		//fprintf(stderr, "device.h: ReadProxy len=%d  maxIndex=%d\n", len.getValue(),  maxIndex.getValue() );

		Index const index(off / sizeof(_Type));
		Count const total(len / sizeof(_Type));

		// Make sure the offset is within range.

		//fprintf(stderr, "device.h: ReadProxy array len=%d index =%d maxIndex=%d total=%d\n", len.getValue(), index.getValue(), maxIndex.getValue(), total.getValue());
		if (index < maxIndex) {

		    // Make sure the length is valid.

		    if (index + total <= maxIndex) {
			Array<_Type> tmp(reinterpret_cast<_Type*>(buf),
					 index, total);

			// All is good. Call the handler (but scale
			// the length and offset into element count
			// and index, respectively.

			(ob.*cb)(tmp, ri);
		    } else{
			fprintf(stderr, "readProxy Array bad length\n");
			throw Ex_BADLEN; //std::runtime_error("bad length");
		    }
		} else
		    throw Ex_BADOFF; //std::runtime_error("bad offset");
	    }

	    // Call the function with no length checking for Array
	    // types.  (Use the default maximum length as the length
	    // here.). This is needed in FTP because the Fast reading
	    // functions don't return a set amount of data, instead
	    // returning as many points as they think are appropriate.

	    void operator()(void* const buf, Index const off, ReqInfo const*ri)
	    {
		//fprintf(stderr, "device.h: ReadProxy len=%d  maxIndex=%d\n", len.getValue(),  maxIndex.getValue() );

		Index const index(off / sizeof(_Type));
		Count const total(maxIndex.getValue() / sizeof(_Type));

		//fprintf(stderr, "device.h: ReadProxy array len= default index =%d maxIndex=%d total=%d\n", index.getValue(), maxIndex.getValue(), total.getValue());
		// Make sure the offset is within range.

		if (index < maxIndex) {

		    // Make sure the length is valid.

		    if (index + total <= maxIndex) {
			Array<_Type> tmp(reinterpret_cast<_Type*>(buf),
					 index, total);

			// All is good. Call the handler (but scale
			// the length and offset into element count
			// and index, respectively.

			(ob.*cb)(tmp,ri);
		    } else{
			fprintf(stderr, "readProxy Array bad length\n");
			throw Ex_BADLEN; //std::runtime_error("bad length");
		    }
		} else
		    throw Ex_BADOFF; //std::runtime_error("bad offset");
	    }
	};

	// Abstract base class that simply defines the interface used
	// by any derived WriteProxy classes.

	class WriteProxyBase {
	 public:
	    virtual ~WriteProxyBase() {}
	    virtual void operator()(void const*, Count, Index, ReqInfo const*) = 0;
	};

	// Templated class which defines specific instances of a
	// WriteProxy. This class turns a generic write which
	// specifies a buffer and length into a specific call to a
	// method that is strongly typed.

	template <class _Obj, class _Type>
	class WriteProxy : public WriteProxyBase {
	 public:
	    typedef void (_Obj::*Callback)(_Type const&,ReqInfo const*);

	 private:
	    _Obj& ob;
	    Callback const cb;

	 public:
	    WriteProxy(_Obj& _ob, Callback _cb) : ob(_ob), cb(_cb) {}

	    void operator()(void const* buf, Count const len, Index const off, ReqInfo const*ri)
	    {

		if (off == Index(0)) {
		    if (len == Count(sizeof(_Type))){
			(ob.*cb)(*reinterpret_cast<_Type const*>(buf),ri);
		    }
		    else{
			fprintf(stderr, "writeProxy bad length\n");
			throw Ex_BADLEN; //std::runtime_error("bad length");
		    }
		} else
		    throw Ex_BADOFF; //std::runtime_error("bad offset");
	    }
	};

	template <class _Obj, class _Type>
	class WriteProxy<_Obj, Array<_Type const> > : public WriteProxyBase {
	 public:
	    typedef void (_Obj::*Callback)(Array<_Type const>&,ReqInfo const*);

	 private:
	    _Obj& ob;
	    Callback const cb;
	    Index const maxIndex;

	 public:
	    WriteProxy(_Obj& _ob, Index const ms, Callback _cb) :
		ob(_ob), cb(_cb), maxIndex(ms)  {}

	    void operator()(void const* const buf, Count const len, Index const off,ReqInfo const* ri)
	    {
		Index const index(off / sizeof(_Type));
		Count const total(len / sizeof(_Type));

		// Make sure the offset is within range.

		if (index < maxIndex) {

		    // Make sure the length is valid.

		    if (index + total <= maxIndex) {
			Array<_Type const> tmp(reinterpret_cast<_Type const*>(buf),
					       index, total);

			// All is good. Call the handler (but scale
			// the length and offset into element count
			// and index, respectively.

			(ob.*cb)(tmp,ri);
		    } else{
		        fprintf(stderr, "writeProxy Array bad length\n");
			throw Ex_BADLEN; //std::runtime_error("bad length");
		    }
		} else{
		    fprintf(stderr, "device.h: Set C++ Exception off=%d index =%d maxIndex=%d total=%d\n", off.getValue(), index.getValue(), maxIndex.getValue(), total.getValue());
		    throw Ex_BADOFF; //std::runtime_error("bad offset");
		}
	    }
	};

	// Use auto_ptrs to manage the memory used by the Read and
	// Write Proxies.

	typedef std::auto_ptr<ReadProxyBase> ReadProxyPtr;
	typedef std::auto_ptr<WriteProxyBase> WriteProxyPtr;

	struct {
	    std::string name;
	    std::string description;
	    ReadProxyPtr reader;
	    WriteProxyPtr writer;
	} method[total1];

	struct {
	    std::string name;
	    std::string description;
	    ReadProxyPtr reader;
	} methodFast[total1];

     protected:
	template <class _Obj, class _Type>
	void registerMethods(size_t const idx, _Obj& o,
			     void (_Obj::*rf)(_Type&, ReqInfo const*),
			     void (_Obj::*wf)(_Type const&, ReqInfo const*))
	{
	    if (idx < total1) {
		method[idx].reader.reset(rf ? new ReadProxy<_Obj, _Type>(o, rf) : 0);
		method[idx].writer.reset(wf ? new WriteProxy<_Obj, _Type const>(o, wf) : 0);
	    } else
		throw Ex_MTVAL; //std::range_error("out of range index for method table");
	}

	template <class _Obj, class _Type>
	void registerMethods(size_t const idx, _Obj& o,
			     void (_Obj::*rf)(Array<_Type>&, ReqInfo const*),
			     void (_Obj::*wf)(Array<_Type const>&, ReqInfo const*),
			     size_t const ms)
	{
	    if (idx < total1) {
		method[idx].reader.reset(rf ? new ReadProxy<_Obj, Array<_Type> >(o, Index(ms), rf) : 0);
		method[idx].writer.reset(wf ? new WriteProxy<_Obj, Array<_Type const> >(o, Index(ms), wf) : 0);
	    } else
		throw Ex_MTVAL; //std::logic_error("out of range index for method table");
	}

	template <class _Obj, class _Type>
	void registerMethod(size_t idx, _Obj& o, void (_Obj::*rf)(_Type&, ReqInfo const*))
	{
	    registerMethods(idx, o, rf, (void (_Obj::*)(_Type const&, ReqInfo const*)) 0);
	}

	template <class _Obj, class _Type>
	void registerMethod(size_t idx, _Obj& o, void (_Obj::*rf)(Array<_Type>&, ReqInfo const*), size_t const ms)
	{
	    registerMethods(idx, o, rf, (void (_Obj::*)(Array<_Type const>&, ReqInfo const*)) 0, ms);
	}

	template <class _Obj, class _Type>
	void registerMethod(size_t idx, _Obj& o, void (_Obj::*wf)(_Type const&, ReqInfo const*))
	{
	    registerMethods(idx, o, (void (_Obj::*)(_Type&,ReqInfo const*)) 0, wf);
	}

	template <class _Obj, class _Type>
	void registerMethod(size_t idx, _Obj& o, void (_Obj::*wf)(Array<_Type const>&, ReqInfo const*), size_t const ms)
	{
	    registerMethods(idx, o, (void (_Obj::*)(Array<_Type>&, ReqInfo const*)) 0, wf, ms);
	}

	template <class _Obj, class _Type>
	void registerMethodFast(size_t const idx, _Obj& o,
				void (_Obj::*rf)(Array<_Type>&, ReqInfo const*),
				size_t const ms)
	{
	    if (idx < total1)
		methodFast[idx].reader.reset(rf ? new ReadProxy<_Obj, Array<_Type> >(o, Index(ms), rf) : 0);
	    else
		throw Ex_MTVAL; //std::logic_error("out of range index for method table");
	}

	Device() {}

     public:
	Device(std::string const& n, std::string const& d) :
	    DeviceBase(n, d) {}

	virtual void getProperty(size_t const idx, void* const buf,
				 Count const len, Index const off, ReqInfo const*ri)
	{
	    if (idx < total1) {
		ReadProxyPtr& fp = method[idx].reader;

		if (fp.get())
		    (*fp)(buf, len, off, ri);
		else{
		    fprintf(stderr, "device.h: getProperty idx=%d, method not defined\n", (int) idx);
		    throw Ex_UNSUPMT; //std::runtime_error("method not defined");
		}
	    } else
		throw Ex_MTVAL; //std::runtime_error("method out of range");
	}

	virtual void setProperty(size_t const idx, void const* const buf,
				 Count const len, Index const off, ReqInfo const*ri)
	{
	    if (idx < total1) {
		WriteProxyPtr& fp = method[idx].writer;

		if (fp.get())
		    (*fp)(buf, len, off, ri);
		else
		    throw Ex_UNSUPMT; //std::runtime_error("method not defined");
	    } else
		throw Ex_MTVAL; //std::runtime_error("method out of range");
	}

	virtual void getPropertyFast(size_t const idx, void* const buf,
				     Index const off, ReqInfo const*ri)
	{
	    //fprintf(stderr, "device.h: gpf len=%d off =%d idx=%d\n", len.getValue(),  off.getValue(), idx );
	    if (idx < total1) {
		ReadProxyPtr& fp = methodFast[idx].reader;

		if (fp.get()) {
		    //fprintf(stderr, "device.h: gpf before actual call \n");
		    (*fp)(buf, off, ri);
		} else {
		    fprintf(stderr, "device.h: getPropertyFast idx=%d method not defined\n",(int) idx);
		    throw Ex_UNSUPMT; //std::runtime_error("method not defined");
		}
	    } else
		throw Ex_MTVAL; //std::runtime_error("method out of range");
	}
    };

    class SafeFloat {
	unsigned char hexval[6];

     public:
	SafeFloat(float const v)
	{
	    memcpy(&hexval[2], reinterpret_cast<const void*>(&v), 4);
	}

	SafeFloat()
	{
	    hexval[0] = hexval[1] = hexval[2] = hexval[3] = hexval[4] = hexval[5] = 0;
	}

	// the default copy assignment does what we want -- copy members
	//     SafeFloat& operator=(SafeFloat& a) { return a;}

	SafeFloat& operator=(float const v)
	{
	    memcpy(&hexval[2], reinterpret_cast<const void*>(&v), 4);
	    // 	fprintf(stderr, " flt val = %d %d %d %d %d %d ",
	    // 		hexval[0], hexval[1], hexval[2], hexval[3], hexval[4], hexval[5]);
	    return *this;
	}

	SafeFloat& operator=(int v)
	{
	    *reinterpret_cast<short*>(hexval) = v;
	    //fprintf(stderr, "device.h: hv0=%d hv1=%d\n", hexval[0], hexval[1]);
	    return *this;
	}

	SafeFloat& operator=(unsigned int v)
	{
	    short *timeptr = reinterpret_cast<short *>(hexval);
	    *timeptr = v;
	    //fprintf(stderr, "device.h: hv0=%d hv1=%d\n", hexval[0], hexval[1]);
	    return *this;
	}

	uint32_t getValue() const { return (hexval[0] << 8) + hexval[1]; }
    };
};

#endif


// Local Variables:
// mode:c++
// End:
