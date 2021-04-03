/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

template <class T>
class ExclusiveGuard
{
public:
	explicit ExclusiveGuard(T& mutex) : target(mutex)
	{
		AcquireSRWLockExclusive(&target);
	}

	explicit ExclusiveGuard(T* mutex) : target(*mutex)
	{
		AcquireSRWLockExclusive(target);
	}

	~ExclusiveGuard()
	{
		ReleaseSRWLockExclusive(&target);
	}

	ExclusiveGuard& operator=(ExclusiveGuard& src)
	{
		this->target = src.target;
		return *this;
	}

protected:
	T& target;
};

typedef ExclusiveGuard<SRWLOCK> ExclusiveSRW;

template <class T>
class SharedGuard
{
public:
	explicit SharedGuard(T& mutex) : target(mutex)
	{
		AcquireSRWLockExclusive(&target);
	}

	explicit SharedGuard(T* mutex) : target(*mutex)
	{
		AcquireSRWLockExclusive(target);
	}

	~SharedGuard()
	{
		ReleaseSRWLockExclusive(&target);
	}

	SharedGuard& operator=(SharedGuard& src)
	{
		this->target = src.target;
		return *this;
	}

protected:
	T& target;
};

typedef SharedGuard<SRWLOCK> SharedSRW;
