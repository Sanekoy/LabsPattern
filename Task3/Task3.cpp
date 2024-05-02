/*Вариант 7 
    Дан рецепт(точнее, состав блюда) : шампиньоны(500г), креветки (100г), сметана(30г), сыр(20г), укроп и пряности(30г).
    Для каждого продукта известны(на 100г продукта) энергетическая ценность, количество белков, жиров и углеводов, а также стоимость 100 г продукта.
    Шампиньоны — 27 кКал, 4.4г белки, 1г жиры, 0.2г углеводы, 20 руб
    Креветки — 83 кКал, 18г белки, 1 г жиры, 0г углеводы, 50 руб
    Сметана — 120 кКал, 3.3г белки, 10 г жиры, 3.3 г углеводы, 14 руб 40 коп
    Сыр — 345 кКал, 25 г белки, 25 г жиры, 0 г углеводы, 70 руб
    Укроп и пряности — 37 кКал, 3.3 г белки, 0 г жиры, 7 г углеводы, 20 руб
    Напишите иерархию классов «Продукты» и класс(или структуру) «Рецепт блюда».
    При помощи паттерна Visitor подсчитайте :
    1. калорийность блюда
    2. количество белков в блюде
    3. количество жиров в блюде
    4. количество углеводов в блюде
    5. стоимость блюда*/

#include <iostream>
#include <vector>
#include <string>
#include <locale.h>
#include <iomanip>
using namespace std;

// Базовый класс для всех продуктов
class Product {
public:
    Product() = default;
    Product(double calories, double proteins, double fats, double carbs, double price)
        : CaloriesPer100g(calories), ProteinsPer100g(proteins), FatsPer100g(fats),
        CarbsPer100g(carbs), PricePer100g(price) {}

    double getCaloriesPer100g() const { return CaloriesPer100g; }
    double getProteinsPer100g() const { return ProteinsPer100g; }
    double getFatsPer100g() const { return FatsPer100g; }
    double getCarbsPer100g() const { return CarbsPer100g; }
    double getPricePer100g() const { return PricePer100g; }

private:
    double CaloriesPer100g; //Энергетическая ценность на 100г
    double ProteinsPer100g; //Кол-во белков на 100г
    double FatsPer100g;     //Кол-во жиров на 100г
    double CarbsPer100g;    //Кол-во углеводов на 100г
    double PricePer100g;    //Стоимость на 100г
};

// Шампиньоны
class Mushrooms : public Product {
public:
    Mushrooms() : Product(27.0, 4.4, 1.0, 0.2, 20.0) {}
};

// Креветки
class Shrimp : public Product {
public:
    Shrimp() : Product(83.0, 18.0, 1.0, 0.0, 50.0) {}
};

// Сметана
class SourCream : public Product {
public:
    SourCream() : Product(120.0, 3.3, 10.0, 3.3, 14.4) {}
};

// Сыр
class Cheese : public Product {
public:
    Cheese() : Product(345.0, 25.0, 25.0, 0.0, 70.0) {}
};

// Укроп и пряности
class Greens : public Product {
public:
    Greens() : Product(37.0, 3.3, 0.0, 7.0, 20.0) {}
};

//Рецепт блюда
class Recipe {
public:
    void addIngredient(const Product* ingredient, double weight) {
        ingredients.push_back(make_pair(ingredient, weight));
    }

    // Посетитель для подсчета характеристик блюда
    class NutritionalInfoVisitor {
    public:
        void visit(const Recipe& recipe) {
            totalCalories = 0.0;
            totalProteins = 0.0;
            totalFats = 0.0;
            totalCarbs = 0.0;
            totalCost = 0.0;
            for (const auto& ingredient : recipe.ingredients) {
                totalCalories += ingredient.first->getCaloriesPer100g() * ingredient.second / 100.0;
                totalProteins += ingredient.first->getProteinsPer100g() * ingredient.second / 100.0;
                totalFats += ingredient.first->getFatsPer100g() * ingredient.second / 100.0;
                totalCarbs += ingredient.first->getCarbsPer100g() * ingredient.second / 100.0;
                totalCost += ingredient.first->getPricePer100g() * ingredient.second / 100.0;
            }
        }

        double getTotalCalories() { return totalCalories; }
        double getTotalProteins() { return totalProteins; }
        double getTotalFats() { return totalFats; }
        double getTotalCarbs() { return totalCarbs; }
        double getTotalCost() { return totalCost; }

    private:
        double totalCalories;   //Энергетическая ценность блюда
        double totalProteins;   //Кол-во белков в блюде
        double totalFats;       //Кол-во жиров в блюде
        double totalCarbs;      //Кол-во углеводов в блюде
        double totalCost;       //Стоимость блюда
    };

    // Метод для принятия посетителя
    void accept(NutritionalInfoVisitor& visitor) {
        visitor.visit(*this);
    }

private:
    vector<pair<const Product*, double>> ingredients; // Вектор пар: продукт и его вес в граммах
};

int main() {
    setlocale(LC_ALL, "Russian");

    // Создание продуктов
    Mushrooms mushrooms;
    Shrimp shrimp;
    SourCream sourCream;
    Cheese cheese;
    Greens greens;

    // Создание рецепта блюда
    Recipe recipe;
    recipe.addIngredient(&mushrooms, 500.0);
    recipe.addIngredient(&shrimp, 100.0);
    recipe.addIngredient(&sourCream, 30.0);
    recipe.addIngredient(&cheese, 20.0);
    recipe.addIngredient(&greens, 30.0);

    Recipe::NutritionalInfoVisitor visitor;

    // Подсчет характеристик блюда
    recipe.accept(visitor);

    // Вывод результатов
    cout << "Характеристики блюда:\n";
    cout << "1. Калорийность:\t\t " << visitor.getTotalCalories() << " kCal\n";
    cout << "2. Количество белков:\t\t " << visitor.getTotalProteins() << " g\n";
    cout << "3. Количество жиров:\t\t " << visitor.getTotalFats() << " g\n";
    cout << "4. Количество углеводов:\t " << visitor.getTotalCarbs() << " g\n";
    cout << "5. Стоимость:\t\t\t " << fixed << setprecision(2) << visitor.getTotalCost() << " rub\n";

    return 0;
}
