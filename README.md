# Проекти Данііла та Ярослава

Цей репозиторій містить невеликі проекти, які створюються разом з моїми дітьми – Даніілом та Ярославом (обоє їм по 9 років). Ми експериментуємо з мікроконтролерами та дисплеями, навчаємося основам програмування та електроніки, а також досліджуємо, як за допомогою апаратних засобів обробляти натискання кнопок та відображати інформацію на екранах.

---

## Зміст

- [Огляд проектів](#огляд-проектів)
- [Використані технології та компоненти](#використані-технології-та-компоненти)
- [Інструкції для роботи](#інструкції-для-роботи)
- [Допоміжні матеріали](#допоміжні-матеріали)
- [Зворотній зв'язок](#зворотній-звязок)
- [Ліцензія](#ліцензія)

---

## Огляд проектів

### Jar1602.X
- **Автор:** Ярослав  
- **Опис:**  
  Проект Jar1602.X використовує LCD дисплей 1602 для відображення інформації, підключений до плати з мікроконтролером PIC. Хоча точна модель плати поки невідома, робота базується на базових принципах роботи з дисплеєм та кнопками.
- **Основні функції:**
  - Відображення текстової інформації на дисплеї.
  - Обробка натискань кнопок для зміни відображуваного вмісту.
  - Реакція на прості вхідні сигнали.

### DanLCD_From_Cond.X
- **Автор:** Данііл  
- **Опис:**  
  Проект DanLCD_From_Cond.X використовує дисплей, який раніше використовувався в кондиціонері. На платі встановлено мікроконтролер HT1621, а управління відбувається через зламану плату Arduino, де використано контролер AT MEGA. Цікаво, що програмування проводиться напряму (без використання стандартної платформи Arduino), що дозволяє краще зрозуміти роботу мікроконтролера.
- **Основні функції:**
  - Відображення інформації з використанням нестандартного дисплею.
  - Реакція на натискання кнопок із обробкою сигналів.
  - Налаштування контролера AT MEGA через пряме програмування.

---

## Використані технології та компоненти

- **Мікроконтролери:**
  - PIC (для проекту Jar1602.X)
  - HT1621 (підключення дисплею в проекті DanLCD_From_Cond.X)
  - AT MEGA (контролер, який програмується напряму в проекті DanLCD_From_Cond.X)
- **Дисплеї:**
  - LCD 1602 (Jar1602.X)
  - Дисплей із кондиціонера (DanLCD_From_Cond.X)
- **Платформи та підходи:**
  - Використання зламаної плати Arduino для DanLCD_From_Cond.X (без застосування стандартної Arduino-платформи)
  - Проста апаратна плата з PIC для Jar1602.X

---

## Інструкції для роботи

1. **Підготовка:**
   - Переконайтеся, що у вас є необхідне обладнання для програмування мікроконтролерів (програматор, мультиметр, осцилограф при потребі).
   - Ознайомтеся з документацією до відповідних мікроконтролерів (PIC, HT1621, AT MEGA).

2. **Перегляд коду:**
   - Кожен з проектів знаходиться в окремій папці. Відкрийте потрібний вам проект для перегляду коду.
   - Коментарі в коді допоможуть зрозуміти основну логіку роботи проекту та пояснюють, як обробляються вхідні сигнали та відображається інформація.

3. **Завантаження та налагодження:**
   - Завантажуйте код на плату відповідно до інструкцій, наданих у кожному з проектів.
   - Тестуйте роботу дисплею та кнопок, експериментуйте з налаштуваннями.

4. **Експерименти:**
   - Заохочується креативний підхід: додавайте нові функції, змінюйте логіку роботи пристроїв, навчайтесь новим методам програмування мікроконтролерів.
   - Документуйте свої зміни та результати експериментів для подальшого аналізу.

---

## Допоміжні матеріали

- **Документація та довідники:**
  - Інформація про мікроконтролери PIC, HT1621 та AT MEGA.
  - Посібники з програмування та роботи з LCD дисплеями.
- **Навчальні ресурси:**
  - Онлайн-курси, відеоуроки та статті про роботу з Arduino та мікроконтролерами.
  - Спільноти та форуми, де можна знайти відповіді на питання та поради для нових проектів.

---

## Зворотній зв'язок

Якщо у вас є будь-які питання або пропозиції щодо проектів, залишайте їх у розділі [Issues](https://github.com/) або звертайтеся безпосередньо. Ми завжди раді новим ідеям та конструктивній критиці!

---

## Ліцензія

Ці проекти розповсюджуються під ліцензією MIT. Детальніше див. файл [LICENSE](LICENSE).

---

*Зроблено з любов’ю та заохоченням допитливості до програмування та електроніки.*
