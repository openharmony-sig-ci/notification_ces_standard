/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common_event_data.h"

#include "event_log_wrapper.h"
#include "string_ex.h"

namespace OHOS {
namespace EventFwk {
CommonEventData::CommonEventData() : code_(0)
{
    EVENT_LOGD("constructor used to create a common event data instance");
}

CommonEventData::CommonEventData(const Want &want) : want_(want), code_(0)
{
    EVENT_LOGD("constructor used to create a common event data instance by want");
}

CommonEventData::CommonEventData(const Want &want, const int &code, const std::string &data)
    : want_(want), code_(code), data_(data)
{
    EVENT_LOGD("constructor used to create a common event data instance by want code data.");
}

CommonEventData::~CommonEventData()
{
    EVENT_LOGD("deconstructor used to destory a common event data instance");
}

void CommonEventData::SetCode(const int &code)
{
    code_ = code;
}

int CommonEventData::GetCode() const
{
    return code_;
}

void CommonEventData::SetData(const std::string &data)
{
    data_ = data;
}

std::string CommonEventData::GetData() const
{
    return data_;
}

void CommonEventData::SetWant(const Want &want)
{
    want_ = want;
}

const Want &CommonEventData::GetWant() const
{
    return want_;
}

bool CommonEventData::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteString16(Str8ToStr16(data_))) {
        EVENT_LOGE("Failed to write data");
        return false;
    }

    if (!parcel.WriteInt32(code_)) {
        EVENT_LOGE("Failed to write code");
        return false;
    }

    if (!parcel.WriteParcelable(&want_)) {
        EVENT_LOGE("Failed to write want");
        return false;
    }

    return true;
}

bool CommonEventData::ReadFromParcel(Parcel &parcel)
{
    // read data
    data_ = Str16ToStr8(parcel.ReadString16());

    // read code
    code_ = parcel.ReadUint32();

    // read want
    std::unique_ptr<Want> want(parcel.ReadParcelable<Want>());
    if (!want) {
        EVENT_LOGE("Failed to ReadParcelable<Want>");
        return false;
    }
    want_ = *want;

    return true;
}

CommonEventData *CommonEventData::Unmarshalling(Parcel &parcel)
{
    CommonEventData *commonEventData = new CommonEventData();

    if (commonEventData && !commonEventData->ReadFromParcel(parcel)) {
        delete commonEventData;
        commonEventData = nullptr;
    }

    return commonEventData;
}
}  // namespace EventFwk
}  // namespace OHOS