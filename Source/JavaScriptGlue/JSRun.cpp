/*
 * Copyright (C) 2005, 2008 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "JSRun.h"

#include "UserObjectImp.h"
#include <JavaScriptCore/Completion.h>
#include <JavaScriptCore/SourceCode.h>

JSGlueGlobalObject::JSGlueGlobalObject(JSGlobalData& globalData, Structure* structure, Structure* userObjectStructure, JSFlags flags)
    : JSGlobalObject(globalData, structure)
    , m_flags(flags)
    , m_userObjectStructure(globalData, userObjectStructure)
{
}

JSRun::JSRun(CFStringRef source, JSFlags inFlags)
    :   JSBase(kJSRunTypeID),
        fSource(CFStringToUString(source)),
        fGlobalObject(getThreadGlobalExecState()->globalData(), JSGlueGlobalObject::create(getThreadGlobalExecState()->globalData(), JSGlueGlobalObject::createStructure(getThreadGlobalExecState()->globalData(), jsNull()), inFlags)),
        fFlags(inFlags)
{
}

JSRun::~JSRun()
{
}

JSFlags JSRun::Flags() const
{
    return fFlags;
}

UString JSRun::GetSource() const
{
    return fSource;
}

JSGlobalObject* JSRun::GlobalObject() const
{
    return fGlobalObject.get();
}

JSValue JSRun::Evaluate(JSValue* expception)
{
    return JSC::evaluate(fGlobalObject->globalExec(), fGlobalObject->globalScopeChain(), makeSource(fSource), JSValue(), expception);
}

bool JSRun::CheckSyntax()
{
    return JSC::checkSyntax(fGlobalObject->globalExec(), makeSource(fSource));
}
