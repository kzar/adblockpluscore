/*
 * This file is part of Adblock Plus <https://adblockplus.org/>,
 * Copyright (C) 2006-2017 eyeo GmbH
 *
 * Adblock Plus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * Adblock Plus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Adblock Plus.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Filter.h"
#include "ActiveFilter.h"

enum class TrippleState {YES, NO, ANY};

struct RegExpFilterData
{
  mutable String::size_type mPatternStart;
  union
  {
    mutable int mRegexpId;
    mutable String::size_type mPatternEnd;
  };
  mutable String::size_type mDomainsStart;
  mutable String::size_type mDomainsEnd;
  mutable String::size_type mSitekeysStart;
  mutable String::size_type mSitekeysEnd;
  int mContentType;
  bool mMatchCase;
  TrippleState mThirdParty;
  TrippleState mCollapse;

  bool RegExpParsingDone() const
  {
    return mPatternStart == String::npos;
  }

  void SetRegExp(int regexpId) const
  {
    mRegexpId = regexpId;
    mPatternStart = String::npos;
  }

  bool HasRegExp() const
  {
    return RegExpParsingDone() && mRegexpId;
  }

  const DependentString GetRegExpSource(const String& text) const
  {
    return DependentString(text, mPatternStart, mPatternEnd - mPatternStart);
  }

  bool DomainsParsingDone() const
  {
    return mDomainsStart == String::npos;
  }

  void SetDomainsParsingDone() const
  {
    mDomainsStart = String::npos;
  }

  const DependentString GetDomainsSource(const String& text) const
  {
    return DependentString(text, mDomainsStart, mDomainsEnd - mDomainsStart);
  }

  bool SitekeyParsingDone() const
  {
    return mSitekeysStart == String::npos;
  }

  void SetSitekeysParsingDone() const
  {
    mSitekeysStart = String::npos;
  }

  const DependentString GetSitekeysSource(const String& text) const
  {
    return DependentString(text, mSitekeysStart, mSitekeysEnd - mSitekeysStart);
  }
};

class RegExpFilter : public ActiveFilter
{
private:
  void ParseSitekeys(const String& sitekeys) const;

protected:
  RegExpFilterData mData;

  DomainMap* GetDomains() const override;
  SitekeySet* GetSitekeys() const override;
public:
  explicit RegExpFilter(Type type, const String& text, const RegExpFilterData& data);
  ~RegExpFilter();
  static Type Parse(DependentString& text, DependentString& error,
      RegExpFilterData& data);
  EMSCRIPTEN_KEEPALIVE static void InitJSTypes();
  EMSCRIPTEN_KEEPALIVE bool Matches(const String& location, int typeMask,
      DependentString& docDomain, bool thirdParty, const String& sitekey) const;
};
