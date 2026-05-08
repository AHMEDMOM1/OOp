# 📋 سجل التعديلات الكامل — مشروع نظام المواقف (Parking System)

---

## ملف `Car.h`

### التعديل 1: إضافة `private` صريح
```diff
-class Car {
-    string _plateNumber;
+class Car {
+private:
+    std::string _plateNumber{};
```
> **السبب:** بدون `private` صريح، قد يُفهم خطأً أن المتغيرات عامة.

---

### التعديل 2: استخدام `Initializer List` في المنشئ
```diff
-Car(string p, string t, string c) {
-    _plateNumber = p;
-    _type = t;
-    _color = c;
-}
+Car(std::string plateNumber, std::string type, std::string color)
+    : _plateNumber(plateNumber), _type(type), _color(color) {}
```
> **السبب:** الـ `Initializer List` أسرع لأنها تبني المتغير مباشرة بدل بنائه ثم إعادة تعيينه.

---

### التعديل 3: إزالة `#include <iostream>` غير المطلوب
```diff
-#include <iostream>
+#include <string>
```
> **السبب:** الكلاس لا يطبع شيئاً، يحتاج فقط `<string>`.

---

## ملف `ParkingSpot.h`

### التعديل 4: تحويل `struct` إلى `class`
```diff
-struct ParkingSpot {
-    int _parkNumber;
-    _ParkState _parkState;
+class ParkingSpot {
+private:
+    int _parkNumber{};
+    ParkState _parkState{};
```
> **السبب:** الـ `struct` يجعل كل شيء `public` افتراضياً، وهذا يكسر مبدأ التغليف (Encapsulation).

---

### التعديل 5: تصحيح إملائي `vocate` → `vacate`
```diff
-void vocate() {
+void vacate() {
```

---

### التعديل 6: إضافة دالة `isFree()`
```diff
+bool isFree() const { return _parkState == ParkState::Free; }
```
> **السبب:** كانت ناقصة تماماً، وهي مطلوبة للبحث عن أول موقف فارغ.

---

### التعديل 7: إزالة `#include "Car.h"` الزائد
```diff
-#include "Car.h"
```
> **السبب:** `ParkingSpot` لا يحتاج أن يعرف عن `Car` أصلاً.

---

### التعديل 8: استخدام `enum class` بدل `enum` عادي
```diff
-enum _ParkState { Free, Occupied };
+enum class ParkState { Free, Occupied };
```
> **السبب:** الـ `enum class` يمنع تضارب الأسماء ويوفر أماناً أكبر للأنواع (Type Safety).

---

## ملف `ParkingRecord.h`

### التعديل 9: تغيير نوع الوقت من `string` إلى `time_t`
```diff
-std::string _entryTime;
-std::string _exitTime;
+time_t _entryTime{};
+time_t _exitTime{};
```
> **السبب:** لا يمكن حساب الفرق الزمني بين نصين! الـ `time_t` يسمح بإجراء عمليات حسابية على الوقت.

---

### التعديل 10: إصلاح `calculateDuration()` التي كانت فارغة
```diff
-std::string calculateDuration() const {
-    return "";
-}
+// تم نقل المنطق إلى Date::getDurationFormatted()
+// ويُستدعى في print() كالتالي:
+Date::getDurationFormatted(_entryTime, _exitTime != 0 ? _exitTime : time(0))
```
> **السبب:** كانت ترجع نصاً فارغاً `""` دائماً.

---

### التعديل 11: إضافة Default Constructor
```diff
+ParkingRecord() = default;
```
> **السبب:** مطلوب لعمل `unordered_map<string, ParkingRecord>` لأن الـ Map يحتاج منشئاً افتراضياً.

---

### التعديل 12: تخزين `spotNumber` بدل كائن `ParkingSpot` كامل
```diff
-ParkingSpot _spot;
+int _spotNumber{};
```
> **السبب:** الـ `ParkingRecord` لا يحتاج أن "يملك" الموقف، بل يحتاج فقط أن "يشير" لرقمه. هذا يحقق مبدأ Association بدل Composition.

---

## ملف `ParkingSystem.h`

### التعديل 13: 🔴 إصلاح Bug خطير — إشغال النسخة بدل الأصل
```diff
-ParkingSpot findFristFreeSpot() {
-    for (int i = 0; i < _parkSpots.size(); i++) {
-        if (_parkSpots[i].isFree()) {
-            return ParkingSpot(i);  // ❌ يرجع نسخة جديدة!
-        }
-    }
-}
-// ثم:
-ParkingSpot spot = findFristFreeSpot();
-spot.occupy();  // ❌ يشغل النسخة والأصل يبقى فارغاً!

+int _findFirstFreeSpotIndex() const {
+    for (int i = 0; i < (int)_parkSpots.size(); i++) {
+        if (_parkSpots[i].isFree()) {
+            return i;  // ✅ يرجع الموقع (index)
+        }
+    }
+    return -1;
+}
+// ثم:
+_parkSpots[spotIndex].occupy();  // ✅ يشغل الأصل مباشرة!
```
> **السبب:** هذا هو أخطر خطأ في المشروع. الدالة كانت ترجع كائناً جديداً (نسخة)، فعند استدعاء `occupy()` عليه، الموقف الأصلي داخل الـ `vector` يبقى "فارغاً" للأبد. النتيجة: كل سيارة جديدة تأخذ نفس الموقف!

---

### التعديل 14: إكمال دالة `EntryCar` التي كانت ناقصة
```diff
-std::string entryTime{};          // ❌ بدون ;
-ParkingRecord recor{car, spot, }  // ❌ غير مكتمل

+_parkSpots[spotIndex].occupy();
+ParkingRecord record(car, _parkSpots[spotIndex].getParkNumber(), time(0));
+_tickets[plateNumber] = record;
```

---

### التعديل 15: إضافة دالة `Exit()` (كانت غير موجودة)
```cpp
void Exit(const std::string& plateNumber) {
    if (_tickets.count(plateNumber) == 0) { /* سيارة غير موجودة */ }

    ParkingRecord record = _tickets[plateNumber];
    record.setExitTime(time(0));

    int spotIndex = record.getSpotNumber() - 1;
    _parkSpots[spotIndex].vacate();

    _archives.push_back(record);
    _tickets.erase(plateNumber);
}
```

---

### التعديل 16: إضافة `displayStatus()` و `printRecords()`
> كانتا في التصميم الأصلي (Pseudo Code) لكنهما غابتا عن الكود.

---

### التعديل 17: إصلاح `#include "UseFullLib.h"` غير الموجود
```diff
-#include "UseFullLib.h"
+#include "ParkingSpot.h"
+#include "ParkingRecord.h"
```

---

## ملف `Date.h`

### التعديل 18: إزالة `using namespace std`
```diff
-using namespace std;
+// استخدام std:: صريحاً في كل مكان
```
> **السبب:** ممارسة سيئة في ملفات Header لأنها تلوّث الـ namespace لكل ملف يعمل `include` لهذا الملف.

---

### التعديل 19: جعل `getDate()` تقبل `time_t` كمعامل
```diff
-static tm getDate() {
-    time_t now = time(nullptr);
+static tm getDate(time_t date = time(nullptr)) {
     tm ltm{};
-    localtime_s(&ltm, &now);
+    localtime_s(&ltm, &date);
     return ltm;
 }
```
> **السبب:** كانت ترجع فقط الوقت الحالي. أصبحت تقبل أي وقت ممرر.

---

### التعديل 20: إضافة `getDurationFormatted()`
```cpp
static std::string getDurationFormatted(time_t from, time_t to = time(nullptr)) {
    int totalSeconds = calculateDuration(from, to);
    int days    = totalSeconds / 86400;
    int hours   = (totalSeconds % 86400) / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;
    // تُرجع: "1 Day, 3 Hours, 41 Minutes, 40 Seconds"
}
```

---

### التعديل 21: إصلاح خطأ القسمة على 360
```diff
-return (int)difftime(to, from) / 360;  // ❌ 360 ليس ساعة!
+return (int)difftime(to, from);         // ✅ الثواني الخام
```
> **السبب:** الساعة = 3600 ثانية وليس 360.

---

## ملف `clsString.h`

### التعديل 22: إصلاح المتغير الثابت `_date`
```diff
-inline static tm _date{ Date::getDate() };  // ❌ يُحسب مرة واحدة!

-static std::string getCurrentDate(time_t date = time(0)) {
-    return to_string(_date.tm_mon + 1) + ...  // ❌ يستخدم _date الثابت!
-}

+static std::string getCurrentDate(time_t date = time(0)) {
+    tm ltm = Date::getDate(date);  // ✅ يستخدم المعامل الممرر
+    return std::to_string(ltm.tm_mday) + ...
+}
```
> **السبب:** المتغير `_date` كان `static` يُحسب مرة واحدة عند بدء البرنامج. أي وقت تمرره للدالة كان يُتجاهل!

---

## ملف `DataStructure.cpp`

### التعديل 23: إنشاء واجهة تفاعلية بدل الاختبار الثابت
```diff
-// كود اختبار ثابت يدخل ويخرج في نفس اللحظة
-parking.EntryCar(Car("ABC-123", "Toyota", "White"));
-parking.Exit("ABC-123");

+// واجهة تفاعلية عبر الـ Console
+ParkingScreen ui = ParkingScreen::setup();
+ui.run();
```
> **السبب:** الاختبار الثابت لا يسمح بقياس المدة الزمنية الحقيقية. الواجهة التفاعلية تسمح بالانتظار بين الدخول والخروج.

---

## ملف `ParkingScreen.h` (جديد)

### التعديل 24: إنشاء طبقة UI مستقلة
```text
[ملف جديد] UI/ParkingScreen.h
```
> **المحتوى:** كلاس `ParkingScreen` يحتوي على:
> - `setup()` — شاشة ترحيب وإعداد السعة.
> - `run()` — حلقة القائمة الرئيسية.
> - `_handleEntry()` — شاشة إدخال سيارة جديدة.
> - `_handleExit()` — شاشة خروج سيارة.
> - `_displayParkingStatus()` — عرض حالة المواقف.
> - `_handleFindTicket()` — البحث عن تذكرة نشطة.
> - `_handlePrintRecords()` — عرض سجلات سيارة (نشطة + أرشيف).
> - `_printTicket()` — طباعة إيصال مواقف منسّق.
> - `_clearScreen()` — مسح الشاشة بين الشاشات.
> - `_pressToContinue()` — انتظار ضغط Enter.

---

## الهيكل النهائي للمشروع

```text
ParkingSystem/
├── DataStructure.cpp          ← نقطة البداية (3 أسطر فقط)
├── Models/                    ← طبقة البيانات
│   ├── Car.h
│   ├── ParkingSpot.h
│   └── ParkingRecord.h
├── Service/                   ← طبقة المنطق
│   └── ParkingSystem.h
├── UI/                        ← طبقة العرض
│   └── ParkingScreen.h
└── UseFullLib/                ← المكتبات المساعدة
    ├── Date.h
    └── clsString.h
```

---

## ملخص إحصائي

| البند | العدد |
|-------|-------|
| إجمالي التعديلات | 24 |
| أخطاء بنائية (Syntax) | 3 |
| أخطاء منطقية (Logic Bugs) | 4 |
| أخطاء إملائية | 2 |
| دوال ناقصة تم إضافتها | 5 |
| ملفات جديدة تم إنشاؤها | 2 |
| تحسينات هندسية | 8 |
