/*Вариант 7 Дан рецепт(точнее, состав блюда) : шампиньоны(500г), креветки (100г), сметана(30г), сыр(20г), укроп и пряности(30г).
    Для каждого продукта известны(на 100г продукта) энергетическая ценность, количество белков, жиров и углеводов, а также стоимость 100 г продукта.
    Шампиньоны — 27 кКал, 4,4г белки, 1г жиры, 0, 2г углеводы, 20 руб
    Креветки — 83 кКал, 18г белки, 1 г жиры, 0г углеводы, 50 руб
    Сметана — 120 кКал, 3,3г белки, 10 г жиры, 3, 3 г углеводы, 14 руб 40 коп
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
using namespace std;

// Базовый класс для всех продуктов
class Product {
public:
    virtual double getCaloriesPer100g() const = 0; //калории на 100г
    virtual double getProteinsPer100g() const = 0; //белки на 100г
    virtual double getFatsPer100g() const = 0; //жиры на 100г
    virtual double getCarbsPer100g() const = 0; //углеводы на 100г
    virtual double getPricePer100g() const = 0; //стоимость
};

// Шампиньоны
class Mushrooms : public Product {
public:
    double getCaloriesPer100g() { return CaloriesPer100g; }
    double getProteinsPer100g() { return ProteinsPer100g; }
    double getFatsPer100g() { return FatsPer100g; }
    double getCarbsPer100g() { return CarbsPer100g; }
    double getPricePer100g() { return PricePer100g; }
private:
    double CaloriesPer100g = 27.0;
    double ProteinsPer100g = 4.4;
    double FatsPer100g = 1.0;
    double CarbsPer100g = 0.2;
    double PricePer100g = 20.0;
};

// Креветки
class Shrimp : public Product {
public:
    double getCaloriesPer100g() { return CaloriesPer100g; }
    double getProteinsPer100g() { return ProteinsPer100g; }
    double getFatsPer100g() { return FatsPer100g; }
    double getCarbsPer100g() { return CarbsPer100g; }
    double getPricePer100g() { return PricePer100g; }
private:
    double CaloriesPer100g = 83.0;
    double ProteinsPer100g = 18.0;
    double FatsPer100g = 1.0;
    double CarbsPer100g = 0.0;
    double PricePer100g = 50.0;
};

// Сметана
class SourCream : public Product {
public:
    double getCaloriesPer100g() { return CaloriesPer100g; }
    double getProteinsPer100g() { return ProteinsPer100g; }
    double getFatsPer100g() { return FatsPer100g; }
    double getCarbsPer100g() { return CarbsPer100g; }
    double getPricePer100g() { return PricePer100g; }
private:
    double CaloriesPer100g = 120.0;
    double ProteinsPer100g = 3.3;
    double FatsPer100g = 1.00;
    double CarbsPer100g = 3.3;
    double PricePer100g = 14.4;
};

// Сыр
class Cheese : public Product {
public:
    double getCaloriesPer100g() { return CaloriesPer100g; }
    double getProteinsPer100g() { return ProteinsPer100g; }
    double getFatsPer100g() { return FatsPer100g; }
    double getCarbsPer100g() { return CarbsPer100g; }
    double getPricePer100g() { return PricePer100g; }
private:
    double CaloriesPer100g = 345.0;
    double ProteinsPer100g = 25.0;
    double FatsPer100g = 25.0;
    double CarbsPer100g = 0.0;
    double PricePer100g = 70.0;
};

// Укроп и пряности
class Greens : public Product {
public:
    double getCaloriesPer100g() { return CaloriesPer100g; }
    double getProteinsPer100g() { return ProteinsPer100g; }
    double getFatsPer100g() { return FatsPer100g; }
    double getCarbsPer100g() { return CarbsPer100g; }
    double getPricePer100g() { return PricePer100g; }
private:
    double CaloriesPer100g = 37.0;
    double ProteinsPer100g = 3.3;
    double FatsPer100g = 0.0;
    double CarbsPer100g = 7.0;
    double PricePer100g = 20.0;
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

        double getTotalCalories() const { return totalCalories; }
        double getTotalProteins() const { return totalProteins; }
        double getTotalFats() const { return totalFats; }
        double getTotalCarbs() const { return totalCarbs; }
        double getTotalCost() const { return totalCost; }

    private:
        double totalCalories;
        double totalProteins;
        double totalFats;
        double totalCarbs;
        double totalCost;
    };

    // Метод для принятия посетителя
    void accept(NutritionalInfoVisitor& visitor) const {
        visitor.visit(*this);
    }

private:
    vector<pair<const Product*, double>> ingredients; // Вектор пар: продукт и его вес в граммах
};

int main() {
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

    double totalCost = visitor.getTotalCost();
    int rub = static_cast<int>(totalCost);
    int kop = static_cast<int>((totalCost - rub) * 100);

    // Вывод результатов
    cout << "Total Calories: " << visitor.getTotalCalories() << " kCal" << endl;
    cout << "Total Proteins: " << visitor.getTotalProteins() << " g" << endl;
    cout << "Total Fats: " << visitor.getTotalFats() << " g" << endl;
    cout << "Total Carbs: " << visitor.getTotalCarbs() << " g" << endl;
    cout << "Total Cost: " << rub << " rub " << kop << " kop" << endl;

    return 0;
}
