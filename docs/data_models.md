# Модели данных

## Перечисление `Gender`
Используется для указания пола пациента.
*Базовый тип:* `uint8_t`

|Значение|Описание|
|--------|--------|
|`Male`|Мужской|
|`Female`|Женский|

В JSON передаётся строкой: `"gender": "male"`.

---
## Перечисление `PatientStatus`
Используется для отображения текущего статуса оказания помощи пациенту.
*Базовый тип:* `uint8_t`

|Значение|Описание|
|--------|--------|
|`OnTheWay`|Находится в карете скорой помощи|
|`Waiting`|Находится в больнице, в очереди в операционную|
|`InSurgery`|Находится на операции|
|`IntensiveCare`|После операции переведён в реанимацию|
|`Died`|Умер|

В JSON передаётся строкой: `"status": "on_the_way"`.

---
## Структура `EmergencyParams`
Содержит информацию о наличии у пострадавшего экстренных состояний

|Поле|Тип|Описание|
|----|---|--------|
|`is_bleeding`|`bool`|Наличие кровотечения|
|`is_extensive_wounds`|`bool`|Наличие обширных ран|
|`is_penetrating_wounds`|`bool`|Наличие проникающих ранений|
|`is_consciousness_depression`|`bool`|Угнетение сознания|
|`is_respiratory_depression`|`bool`|Угнетение дыхания|
|`is_hemodynamic_depression`|`bool`|Угнетение гемодинамики|
|`is_severe_combined_injury`|`bool`|Тяжёлая сочетанная травма|

---
## Структура `TriageData`
Содержит значения параметров для триажа по шкале `SORT`
Более подбробная информация о методике триажа `SORT` указана [здесь](https://moscow.surgery/glavnyj-hirurg-moskvy/prikaz-po-ekstrennoj-hirurgicheskoj-pomoshhi/algoritm-provedeniya-mediczinskoj-sortirovki-na-gospitalnom-etape/) и [здесь](https://tacmed.pro/algoritmy-sortirovki/)

|Поле|Тип|Диапазон значений|Описание|
|----|---|-----------------|--------|
|`eye_response`|`uint8_t`|[1..4]|Реакция глаз по шкале комы `ГЛАЗГО`|
|`verbal_response`|`uint8_t`|[1..5]|Речевая реакция по шкале комы `ГЛАЗГО`|
|`motor_response`|`uint8_t`|[1..6]|Моторная реакция по шкале комы `ГЛАЗГО`|
|`respiratory_rate`|`uint8_t`|[0..60]|Частота дыхательных движений|
|`systolic_bp`|`uint16_t`|[0..300]|Систолическое артериальное давление|
---
## Структура `DemographyData`
Содержит демографические показатели пациента. Показатели не являются обязательными.

|Поле|Тип|Диапазон значений|
|----|---|-----------------|
|`age`|`std::optional<uint8_t>`|[0..130]|
|`sex`|`std::optional<Gender>`|{Male, Female}|
---

## Структура `AmbulanceData`
Объединяет в себе все данные, получаемые бэкенд-составляющей от клиента `Скорая помощь`
|Поле|Тип|
|----|---|
|`emergency_params`|`EmergencyParams`|
|`triage_data`|`TriageData`|
|`demography_data`|`DemographyData`|


---

## Класс `Patient`
Содержит информацию о пострадавшем, ожидающем операции. Реализована инкапсуляций полей. Публичный интерфейс содержит методы `set` и `get`.

### Поля класса
|Поле|Тип|Описание|
|----|---|--------|
|`timestamp_`|`std::chrono::system_clock::time_point`|Время создания запроса (серверное)|
|`id_`|`uint32_t`|Уникальный идентификатор пациента (автоинкрементный)|
|`priority_`|`uint8_t`|Приоритет пациента (чем меньше число, тем выше приоритет). Вычисляется бизнес-логикой на основе `EmergencyParams` и `TriageData`|
|`status_`|`PatientStatus`|Статус пациента, подробнее ([см. выше](#перечисление-status))|
|`emergency_params_`|`EmergencyParams`|Информация о наличии у пострадавшего экстренных состояний, подробнее ([см. выше](#структура-emergencyparams))|
|`triage_data_`|`TriageData`|Значения параметров для триажа по шкале `SORT`, подробнее ([см. выше](#структура-triagedata))|
|`demography_data_`|`DemographyData`|Демографические показатели, подробнее ([см. выше](#структура-demographydata))|

### Публичный интерфейс
См. [здесь](/src/storage/storage.hpp)

### Формат представления `timestamp` в `JSON`
Количество миллисекунд с `1970-01-01 UTC`. Например, `"timestamp": 1717588800000`

### JSON для запроса `POST /patients`
#### JSON, соответствующий запросу
```json
{
    "emergency_params": {
        "is_bleeding": false,
        "is_extensive_wounds": false,
        "is_penetrating_wounds": false,
        "is_consciousness_depression": false,
        "is_respiratory_depression": false,
        "is_hemodynamic_depression": false,
        "is_severe_combined_injury": false
    },
    "triage_data": {
        "eye_response": 2,
        "verbal_response": 2,
        "motor_response": 2,
        "respiratory_rate": 25,
        "systolic_bp": 120
    },
    "demography_data": {
        "age": 32,
        "sex": "male"
    }
}
```

#### JSON, соответствующий ответу
```json
{
    "id": 123,                  
    "estimated_wait_time": 15   // uint16_t - время в минутах
}
```

### JSON для запроса `GET /patients/{id}`

#### JSON, соответствующий запросу
Не используется, `ID` выделяется из самого HTTP-запроса

#### JSON, соответствующий ответу
```json
{
  "id": 1,
  "medical_data": {
    "demography_data": {
      "age": 32,
      "sex": "male"
    },
    "emergency_params": {
      "is_bleeding": false,
      "is_consciousness_depression": false,
      "is_extensive_wounds": false,
      "is_hemodynamic_depression": false,
      "is_penetrating_wounds": false,
      "is_respiratory_depression": false,
      "is_severe_combined_injury": false
    },
    "triage_data": {
      "eye_response": 2,
      "motor_response": 2,
      "verbal_response": 2,
      "respiratory_rate": 25,
      "systolic_bp": 120
    }
  },
  "priority": 1,
  "status": "on_the_way",
  "timestamp": 1781864756743
}
```

### JSON для ответа на запрос `GET /patients`
```json
[
  {
    "id": 1,
    "medical_data": {
      "demography_data": {
        "age": 32,
        "sex": "male"
      },
      "emergency_params": {
        "is_bleeding": false,
        "is_consciousness_depression": false,
        "is_extensive_wounds": false,
        "is_hemodynamic_depression": false,
        "is_penetrating_wounds": false,
        "is_respiratory_depression": false,
        "is_severe_combined_injury": false
      },
      "triage_data": {
        "eye_response": 2,
        "motor_response": 2,
        "verbal_response": 2,
        "respiratory_rate": 25,
        "systolic_bp": 120
      }
    },
    "priority": 1,
    "status": "on_the_way",
    "timestamp": 1781864756743
  },
  {
    "id": 2,
    "medical_data": {
      "demography_data": {},
      "emeregency_params": {
        "is_bleeding": true,
        "is_consciousness_depression": false,
        "is_extensive_wounds": false,
        "is_hemodynamic_depression": false,
        "is_penetrating_wounds": false,
        "is_respiratory_depression": false,
        "is_severe_combined_injury": false
      },
      "triage_data": {
        "eye_response": 1,
        "motor_response": 4,
        "verbal_response": 2,
        "respiratory_rate": 25,
        "systolic_bp": 120
      }
    },
    "priority": 2,
    "status": "on_the_way",
    "timestamp": 1781864783743
  }
]

```
