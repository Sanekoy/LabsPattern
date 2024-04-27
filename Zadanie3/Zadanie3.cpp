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
    double getCaloriesPer100g() const { return 27.0; }
    double getProteinsPer100g() const { return 4.4; }
    double getFatsPer100g() const { return 1.0; }
    double getCarbsPer100g() const { return 0.2; }
    double getPricePer100g() const { return 20.0; }
};

// Креветки
class Shrimp : public Product {
public:
    double getCaloriesPer100g() const { return 83.0; }
    double getProteinsPer100g() const { return 18.0; }
    double getFatsPer100g() const { return 1.0; }
    double getCarbsPer100g() const { return 0.0; }
    double getPricePer100g() const { return 50.0; }
};

// Сметана
class SourCream : public Product {
public:
    double getCaloriesPer100g() const { return 120.0; }
    double getProteinsPer100g() const { return 3.3; }
    double getFatsPer100g() const { return 10.0; }
    double getCarbsPer100g() const { return 3.3; }
    double getPricePer100g() const { return 14.4; }
};

// Сыр
class Cheese : public Product {
public:
    double getCaloriesPer100g() const { return 345.0; }
    double getProteinsPer100g() const { return 25.0; }
    double getFatsPer100g() const { return 25.0; }
    double getCarbsPer100g() const { return 0.0; }
    double getPricePer100g() const { return 70.0; }
};

// Укроп и пряности
class Greens : public Product {
public:
    double getCaloriesPer100g() const { return 37.0; }
    double getProteinsPer100g() const { return 3.3; }
    double getFatsPer100g() const { return 0.0; }
    double getCarbsPer100g() const { return 7.0; }
    double getPricePer100g() const { return 20.0; } 
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
