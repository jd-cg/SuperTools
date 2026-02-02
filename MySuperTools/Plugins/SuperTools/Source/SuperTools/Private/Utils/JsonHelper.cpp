// Copyright lostpanda. All Rights Reserved.

#include "Utils/JsonHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

bool FJsonHelper::ParseJson(const FString& JsonString, TSharedPtr<FJsonObject>& OutJsonObject)
{
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	return FJsonSerializer::Deserialize(Reader, OutJsonObject);
}

TSharedPtr<FJsonObject> FJsonHelper::GetNestedObject(TSharedPtr<FJsonObject> RootObject, const FString& Path, FString& OutFieldName)
{
	if (!RootObject.IsValid())
	{
		return nullptr;
	}

	TArray<FString> PathParts;
	Path.ParseIntoArray(PathParts, TEXT("."), true);

	if (PathParts.Num() == 0)
	{
		return nullptr;
	}

	// 最后一个是字段名
	OutFieldName = PathParts.Last();
	PathParts.RemoveAt(PathParts.Num() - 1);

	// 遍历路径获取嵌套对象
	TSharedPtr<FJsonObject> CurrentObject = RootObject;
	for (const FString& Part : PathParts)
	{
		const TSharedPtr<FJsonObject>* NestedObject;
		if (!CurrentObject->TryGetObjectField(Part, NestedObject))
		{
			return nullptr;
		}
		CurrentObject = *NestedObject;
	}

	return CurrentObject;
}

FString FJsonHelper::GetStringField(const FString& JsonString, const FString& FieldName, const FString& DefaultValue)
{
	TSharedPtr<FJsonObject> JsonObject;
	if (!ParseJson(JsonString, JsonObject))
	{
		return DefaultValue;
	}

	FString ActualFieldName;
	TSharedPtr<FJsonObject> TargetObject = GetNestedObject(JsonObject, FieldName, ActualFieldName);
	if (!TargetObject.IsValid())
	{
		return DefaultValue;
	}

	FString Value;
	if (TargetObject->TryGetStringField(ActualFieldName, Value))
	{
		return Value;
	}
	return DefaultValue;
}

int32 FJsonHelper::GetIntField(const FString& JsonString, const FString& FieldName, int32 DefaultValue)
{
	TSharedPtr<FJsonObject> JsonObject;
	if (!ParseJson(JsonString, JsonObject))
	{
		return DefaultValue;
	}

	FString ActualFieldName;
	TSharedPtr<FJsonObject> TargetObject = GetNestedObject(JsonObject, FieldName, ActualFieldName);
	if (!TargetObject.IsValid())
	{
		return DefaultValue;
	}

	int32 Value;
	if (TargetObject->TryGetNumberField(ActualFieldName, Value))
	{
		return Value;
	}
	return DefaultValue;
}

float FJsonHelper::GetFloatField(const FString& JsonString, const FString& FieldName, float DefaultValue)
{
	TSharedPtr<FJsonObject> JsonObject;
	if (!ParseJson(JsonString, JsonObject))
	{
		return DefaultValue;
	}

	FString ActualFieldName;
	TSharedPtr<FJsonObject> TargetObject = GetNestedObject(JsonObject, FieldName, ActualFieldName);
	if (!TargetObject.IsValid())
	{
		return DefaultValue;
	}

	double Value;
	if (TargetObject->TryGetNumberField(ActualFieldName, Value))
	{
		return static_cast<float>(Value);
	}
	return DefaultValue;
}

bool FJsonHelper::GetBoolField(const FString& JsonString, const FString& FieldName, bool DefaultValue)
{
	TSharedPtr<FJsonObject> JsonObject;
	if (!ParseJson(JsonString, JsonObject))
	{
		return DefaultValue;
	}

	FString ActualFieldName;
	TSharedPtr<FJsonObject> TargetObject = GetNestedObject(JsonObject, FieldName, ActualFieldName);
	if (!TargetObject.IsValid())
	{
		return DefaultValue;
	}

	bool Value;
	if (TargetObject->TryGetBoolField(ActualFieldName, Value))
	{
		return Value;
	}
	return DefaultValue;
}

bool FJsonHelper::GetStringArrayField(const FString& JsonString, const FString& FieldName, TArray<FString>& OutArray)
{
	OutArray.Empty();

	TSharedPtr<FJsonObject> JsonObject;
	if (!ParseJson(JsonString, JsonObject))
	{
		return false;
	}

	FString ActualFieldName;
	TSharedPtr<FJsonObject> TargetObject = GetNestedObject(JsonObject, FieldName, ActualFieldName);
	if (!TargetObject.IsValid())
	{
		return false;
	}

	const TArray<TSharedPtr<FJsonValue>>* ArrayField;
	if (!TargetObject->TryGetArrayField(ActualFieldName, ArrayField))
	{
		return false;
	}

	for (const TSharedPtr<FJsonValue>& Value : *ArrayField)
	{
		FString StringValue;
		if (Value->TryGetString(StringValue))
		{
			OutArray.Add(StringValue);
		}
	}

	return true;
}

TSharedPtr<FJsonObject> FJsonHelper::CreateJsonObject()
{
	return MakeShareable(new FJsonObject());
}

void FJsonHelper::SetStringField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, const FString& Value)
{
	if (JsonObject.IsValid())
	{
		JsonObject->SetStringField(FieldName, Value);
	}
}

void FJsonHelper::SetIntField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, int32 Value)
{
	if (JsonObject.IsValid())
	{
		JsonObject->SetNumberField(FieldName, Value);
	}
}

void FJsonHelper::SetFloatField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, float Value)
{
	if (JsonObject.IsValid())
	{
		JsonObject->SetNumberField(FieldName, Value);
	}
}

void FJsonHelper::SetBoolField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, bool Value)
{
	if (JsonObject.IsValid())
	{
		JsonObject->SetBoolField(FieldName, Value);
	}
}

FString FJsonHelper::JsonObjectToString(TSharedPtr<FJsonObject> JsonObject, bool bPrettyPrint)
{
	if (!JsonObject.IsValid())
	{
		return TEXT("{}");
	}

	FString OutputString;
	if (bPrettyPrint)
	{
		TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&OutputString);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	}
	else
	{
		TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutputString);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	}

	return OutputString;
}

FString FJsonHelper::MakeJsonString(const FString& Key, const FString& Value)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonObject();
	SetStringField(JsonObject, Key, Value);
	return JsonObjectToString(JsonObject, false);
}

FString FJsonHelper::MapToJsonString(const TMap<FString, FString>& StringMap, bool bPrettyPrint)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonObject();
	for (const auto& Pair : StringMap)
	{
		JsonObject->SetStringField(Pair.Key, Pair.Value);
	}
	return JsonObjectToString(JsonObject, bPrettyPrint);
}

bool FJsonHelper::JsonStringToMap(const FString& JsonString, TMap<FString, FString>& OutMap)
{
	OutMap.Empty();

	TSharedPtr<FJsonObject> JsonObject;
	if (!ParseJson(JsonString, JsonObject))
	{
		return false;
	}

	for (const auto& Pair : JsonObject->Values)
	{
		FString StringValue;
		if (Pair.Value->TryGetString(StringValue))
		{
			OutMap.Add(Pair.Key, StringValue);
		}
	}

	return true;
}
