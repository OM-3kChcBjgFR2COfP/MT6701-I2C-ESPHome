# MT6701 для ESPHome (ESP32-WROOM)

Пример опроса магнитного энкодера **MT6701** в **ESPHome** (**ESP32-WROOM**) в качестве `external_component`.

📄 Даташит:  
https://www.novosns.com/enfiles/MT6701_Rev.1.8.pdf

 ---

- 📐 Угол поворота (Angle)  
- 🔢 Сырое значение (Raw)  
- 📊 Статус датчика (Status)  
- 🎯 Детекцию нулевой позиции (Zero Position)  

---

## ⚙️ Подключение

### I²C параметры по умолчанию:

- Адрес: `0x06`  
- Интерфейс: I²C  
- Частота обновления: настраивается через `update_interval`  

---

## 🧩 Пример конфигурации YAML

```yaml

external_components:
  - source:
      type: local
      path: mt6701
    components: [mt6701]

mt6701:
  id: mt6701_1
  i2c_id: bus_b
  address: 0x06
  update_interval: 1s

sensor:
  - platform: mt6701
    mt6701_id: mt6701_1
    angle:
      name: "MT6701 Angle"
      id: mt6701_angle_1
    raw:
      name: "MT6701 Raw"
      id: mt_raw
    status:
      name: "MT6701 Status"
      id: mt_status
      
binary_sensor:
  - platform: mt6701
    mt6701_id: mt6701_1
    name: "Minute Zero Position"
    id: minute_zero
    on_press:
      - logger.log: "Zero detected!"
    on_release:
      - logger.log: "Left zero position"
```

---

## ⚡ Быстрый опрос (Fast Polling Mode)

Компонент поддерживает режим повышенной частоты опроса через функцию:

```cpp
id(mt6701_minute).set_fast_polling(false);
```

### Что делает эта функция:

- 🔄 **Увеличивает частоту опроса датчика**  
- 🚫 **Отключает публикацию сущностей в Home Assistant**  
- ⚡ Позволяет использовать датчик для высокоскоростной локальной логики (например, управление мотором)  

> ⚠ В режиме fast polling данные не публикуются в Home Assistant для уменьшения нагрузки и задержек.

### Пример использования

```yaml
on_boot:
  priority: -100
  then:
    - lambda: |-
        id(mt6701).set_fast_polling(true/false);
```

---

## 📊 Доступные сущности

### Sensor

| Параметр | Описание |
|----------|----------|
| `angle`  | Угол поворота в градусах |
| `raw`    | Сырое 14-битное значение |
| `status` | Статус регистра |

### Binary Sensor

| Параметр | Описание |
|----------|----------|
| Zero Position | Срабатывает при прохождении нулевой позиции |
