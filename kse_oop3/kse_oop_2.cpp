#include <iostream>
#include <vector>
#include <string>
#include "config.h"
#include <memory>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <filesystem>

class User
{
public:
	User()
	{

	}

	int id;
};

class Product
{
public:
	Product()
	{

	}

	virtual std::string getType() const
	{
		return "Product";
	}

	virtual std::string getProductInfo() const
	{
		return "Product: " + name + ", Price: " + std::to_string(price);
	}

	int id = 0;
	std::string type = "";
	std::string name = "";
	double price = 0.0;
};

class ProductCatalog
{
public:
	ProductCatalog()
	{

	}
	std::vector<std::pair<std::unique_ptr<Product>, int>> products;
};

class Electronics : public Product
{
public:
	std::string getProductInfo() const override
	{
		return "Electronics: " + name + ", Brand: " + brand + ", Model: " + model;
	}

	std::string getType() const override
	{
		return "Electronics";
	}

	Electronics() : Product()
	{
		type = "electronics";
	}

	std::string getBrand()
	{
		return brand;
	}

	void setBrand(const std::string& brandN)
	{
		brand = brandN;
	}

	std::string getModel()
	{
		return model;
	}

	void setModel(const std::string& modelN)
	{
		model = modelN;
	}

	double getPowerConsumption()
	{
		return powerConsumption;
	}

	void setPowerConsumption(const double& pC)
	{
		powerConsumption = pC;
	}

	void deserialize(const std::string& data) {
	    std::istringstream iss(data);
	    iss >> type >> name >> price >> brand >> model >> powerConsumption;

	    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
	}

private:
	std::string brand = "";
	std::string model = "";
	double powerConsumption = 0.0;
};

class Books : public Product
{

public:
	Books() : Product()
	{
		type = "book";
	}

	std::string getAuthor()
	{
		return author;
	}

	void setAuthor(const std::string& aut)
	{
		author = aut;
	}

	std::string getGenre()
	{
		return genre;
	}

	void setGenre(const std::string& genree)
	{
		genre = genree;
	}

	std::string getISBN()
	{
		return ISBN;
	}

	void setISBN(const std::string& isbn)
	{
		ISBN = isbn;
	}

	void deserialize(const std::string& data) {
		std::istringstream iss(data);
		iss >> type >> name >> price >> author >> genre >> ISBN;

		std::transform(type.begin(), type.end(), type.begin(), ::tolower);
	}

private:
	std::string author = "";
	std::string genre = "";
	std::string ISBN = "";
};

class Clothing : public Product
{
public:
	Clothing() : Product()
	{
		type = "clothing";
	}

	std::string getSize()
	{
		return size;
	}

	void setSize(const std::string& sizee)
	{
		size = sizee;
	}

	std::string getColor()
	{
		return color;
	}

	void setColor(const std::string& coloor)
	{
		color = coloor;
	}

	std::string getMaterial()
	{
		return material;
	}

	void setMaterial(const std::string& mat)
	{
		material = mat;
	}

	void deserialize(const std::string& data) {
		std::istringstream iss(data);
		iss >> type >> name >> price >> size >> color >> material;

		std::transform(type.begin(), type.end(), type.begin(), ::tolower);
	}

private:
	std::string size = "";
	std::string color = "";
	std::string material = "";
};

class Inventory
{

public:
	Inventory()
	{

		std::ifstream fileInput("/home/artem/eclipse-workspace/cppdefault/kse_oop_2/config.txt");
		std::string buf;

		if (!fileInput.is_open())
		{
			std::cout << "jugg";
		}

		while (std::getline(fileInput, buf)) {
			std::istringstream iss(buf);
			std::string type;
			iss >> type;

			auto createInstance = [](const std::string& type) -> int
			{
				if (type == "Electronics") {
					return 1;
				} else if (type == "Books") {
					return 2;
				} else if (type == "Clothing") {
					return 3;
				}
			};

			std::unique_ptr<Product> product;

			switch (createInstance(type))
			{
			case 1:
				product = std::make_unique<Electronics>();
				static_cast<Electronics*>(product.get())->deserialize(buf);
				break;
			case 2:
				product = std::make_unique<Books>();
				static_cast<Books*>(product.get())->deserialize(buf);
				break;
			case 3:
				product = std::make_unique<Clothing>();
				static_cast<Clothing*>(product.get())->deserialize(buf);
				break;
			}

			products.push_back(std::make_pair(std::move(product), 1));
		}
	}

	std::vector<std::pair<std::unique_ptr<Product>, int>> products;
};

class Order
{
public:
	Order(std::unique_ptr<Product>& x, int q)
	{
		prod = std::move(x);
		quantity = q;
	}
	int quantity;
	std::unique_ptr<Product> prod;
};

class System
{
public:
	System()
	{
	}
	ProductCatalog catalog;

	ProductCatalog* getProdCatalogPtr()
	{
		return &catalog;
	}

	void init_catalog()
	{
		catalog.products.clear();
		for (const auto& product : inv.products)
		{
			std::unique_ptr<Product> clonedProduct;
			if (product.first->type == "electronics")
			{
				clonedProduct = std::make_unique<Electronics>(*static_cast<Electronics*>(product.first.get()));
			}
			else if (product.first->type == "books")
			{
				clonedProduct = std::make_unique<Books>(*static_cast<Books*>(product.first.get()));
			}
			else if (product.first->type == "clothing")
			{
				clonedProduct = std::make_unique<Clothing>(*static_cast<Clothing*>(product.first.get()));
			}

			catalog.products.push_back(std::make_pair(std::move(clonedProduct), product.second));
		}
	}

	Inventory* getInventoryPtr()
	{
		return &inv;
	}

private:
	Inventory inv;
	std::vector<Order> orders;
};

class Admin : public User
{
public:
	Admin(Inventory* catPtr) : User()
	{
		id = 1337;
		catalogPtr = catPtr;
	}

	void show_polymorphism()
	{
		for (const auto& product : catalogPtr->products)
		{
			if (product.first->getType() == "Electronics")
			{
				Electronics* elptrDynamic = dynamic_cast<Electronics*>(product.first.get());

				if (elptrDynamic)
				{
					std::cout << "example of ptr usage from dynamic cast: " << elptrDynamic->getBrand() << "\n";
				}
			}
		}
	}

	void change_prod_quantity(const std::string& type, const std::string& name, int quantity)
	{
		for (auto& product : catalogPtr->products)
		{
			if (product.first->type == type)
			{
				if (product.first->name == name)
				{
					product.second = quantity;
				}
			}
		}
	}
	void add_product_to_inventory(const std::string&typee, const std::string& namee, const double& pricee, const std::vector<std::string>& details)
	{
		auto createInstance = [](const std::string& type) -> int
		{
			if (type == "Electronics") {
				return 1;
			} else if (type == "Books") {
				return 2;
			} else if (type == "Clothing") {
				return 3;
			}
		};

		std::unique_ptr<Product> product;

		switch (createInstance(typee))
		{
		case 1:
			product = std::make_unique<Electronics>();
			product->name = namee;
			product->price = pricee;
			static_cast<Electronics*>(product.get())->setBrand(details[0]);
			static_cast<Electronics*>(product.get())->setModel(details[1]);
			static_cast<Electronics*>(product.get())->setPowerConsumption(std::stod(details[2]));
			break;
		case 2:
			product = std::make_unique<Books>();
			product->name = namee;
			product->price = pricee;
			static_cast<Books*>(product.get())->setAuthor(details[0]);
			static_cast<Books*>(product.get())->setGenre(details[1]);
			static_cast<Books*>(product.get())->setISBN(details[2]);
			break;
		case 3:
			product = std::make_unique<Clothing>();
			product->name = namee;
			product->price = pricee;
			static_cast<Clothing*>(product.get())->setSize(details[0]);
			static_cast<Clothing*>(product.get())->setColor(details[1]);
			static_cast<Clothing*>(product.get())->setMaterial(details[2]);
			break;
		}

		catalogPtr->products.push_back(std::make_pair(std::move(product), 1));
	}

private:
	Inventory* catalogPtr;
};

class Customer : public User
{
public:
	Customer(ProductCatalog* catPtr) : User()
	{
		catalogPtr = catPtr;
		id = 1;
	}

	std::string browse(const std::string& filter)
	{
		std::string output = "";

		if (filter == "show all")
		{
			for (const auto& product : catalogPtr->products)
			{
				if (product.first->type == "electronics")
				{
					auto el = static_cast<Electronics*>(product.first.get());

					std::string s = "type: Electronics, name: " + product.first->name + ", price: " + std::to_string(product.first->price) + ", brand: " + el->getBrand() + ", model: " + el->getModel() + ", powerConsumption: " + std::to_string(el->getPowerConsumption()) + "\n";
					output += s;
				}

				if (product.first->type == "books")
				{
					auto el = static_cast<Books*>(product.first.get());

					std::string s = "type: Book, name: " + product.first->name + ", price: " + std::to_string(product.first->price) + ", author: " + el->getAuthor() + ", genre: " + el->getGenre() + ", isbn: " + el->getISBN() + "\n";
					output += s;
				}

				if (product.first->type == "clothing")
				{
					auto el = static_cast<Clothing*>(product.first.get());

					std::string s = "type: Clothing, name: " + product.first->name + ", price: " + std::to_string(product.first->price) + ", size: " + el->getSize() + ", color: " + el->getColor() + ", material: " + el->getMaterial() + "\n";
					output += s;
				}
			}
		}

		else if (filter == "show clothing shoes")
		{
			for (const auto& product : catalogPtr->products)
			{
				if (product.first->type == "clothing")
				{
					if (product.first->name == "shoes")
					{
						auto el = static_cast<Clothing*>(product.first.get());
						std::string s = "type: Clothing, name: " + product.first->name + ", price: " + std::to_string(product.first->price) + ", size: " + el->getSize() + ", color: " + el->getColor() + ", material: " + el->getMaterial() + "\n";
						output += s;
					}
				}
			}
		}

		else if (filter == "show clothing medium") // + cin type
		{
			std::string typeClothes;
			std::cin >> typeClothes;

			for (const auto& product : catalogPtr->products)
			{
				if (product.first->type == "clothing")
				{
					if (product.first->name == typeClothes)
					{
						auto el = static_cast<Clothing*>(product.first.get());
						if (el->getSize() == "medium")
						{
							std::string s = "type: Clothing, name: " + product.first->name + ", price: " + std::to_string(product.first->price) + ", size: " + el->getSize() + ", color: " + el->getColor() + ", material: " + el->getMaterial() + "\n";
							output += s;
						}
					}
				}
			}
		}

		else if (filter == "show books author")
		{
			std::string typeAuthor;
			std::cin >> typeAuthor;

			for (const auto& product : catalogPtr->products)
			{
				if (product.first->type == "books")
				{
					auto el = static_cast<Books*>(product.first.get());

					if (el->getAuthor() == typeAuthor)
					{
						std::string s = "type: Book, name: " + product.first->name + ", price: " + std::to_string(product.first->price) + ", author: " + el->getAuthor() + ", genre: " + el->getGenre() + ", isbn: " + el->getISBN() + "\n";
						output += s;
					}
				}
			}
		}

		return output;
	}

	void create_order(const std::string& typee, const std::string& namee, const double& q, const std::vector<std::string>& details)
	{
		auto createInstance = [](const std::string& type) -> int
		{
			if (type == "Electronics") {
				return 1;
			} else if (type == "Books") {
				return 2;
			} else if (type == "Clothing") {
				return 3;
			}
		};

		std::unique_ptr<Product> product;

		switch (createInstance(typee))
		{
		case 1:
			product = std::make_unique<Electronics>();
			product->name = namee;
			static_cast<Electronics*>(product.get())->setBrand(details[0]);
			static_cast<Electronics*>(product.get())->setModel(details[1]);
			static_cast<Electronics*>(product.get())->setPowerConsumption(std::stod(details[2]));
			break;
		case 2:
			product = std::make_unique<Books>();
			product->name = namee;
			static_cast<Books*>(product.get())->setAuthor(details[0]);
			static_cast<Books*>(product.get())->setGenre(details[1]);
			static_cast<Books*>(product.get())->setISBN(details[2]);
			break;
		case 3:
			product = std::make_unique<Clothing>();
			product->name = namee;
			static_cast<Clothing*>(product.get())->setSize(details[0]);
			static_cast<Clothing*>(product.get())->setColor(details[1]);
			static_cast<Clothing*>(product.get())->setMaterial(details[2]);
			break;
		}

		cart.push_back(Order(product, q));
	}

	void view_orders()
	{
		for (const auto& product : cart)
		{
			if (product.prod->type == "electronics")
			{
				auto el = static_cast<Electronics*>(product.prod.get());

				std::string s = "type: Electronics, name: " + product.prod->name + ", price: " + std::to_string(product.prod->price) + ", brand: " + el->getBrand() + ", model: " + el->getModel() + ", powerConsumption: " + std::to_string(el->getPowerConsumption()) + "\n";
				std::cout << s;
				std::cout << "quantity " << product.quantity << "\n\n";
			}

			if (product.prod->type == "books")
			{
				auto el = static_cast<Books*>(product.prod.get());

				std::string s = "type: Book, name: " + product.prod->name + ", price: " + std::to_string(product.prod->price) + ", author: " + el->getAuthor() + ", genre: " + el->getGenre() + ", isbn: " + el->getISBN() + "\n";
				std::cout << s;
				std::cout << "quantity " << product.quantity << "\n\n";
			}

			if (product.prod->type == "clothing")
			{
				auto el = static_cast<Clothing*>(product.prod.get());

				std::string s = "type: Clothing, name: " + product.prod->name + ", price: " + std::to_string(product.prod->price) + ", size: " + el->getSize() + ", color: " + el->getColor() + ", material: " + el->getMaterial() + "\n";
				std::cout << s;
				std::cout << "quantity " << product.quantity << "\n\n";
			}
		}
	}

private:
	ProductCatalog* catalogPtr;
	std::vector<Order> cart;
};

int main(int argc, char **argv) {
	System syst;

	syst.init_catalog();

	Customer cust(syst.getProdCatalogPtr());
	Admin adm(syst.getInventoryPtr());

	while (1)
	{
		adm.show_polymorphism();


		std::string role;
		std::cout << "\ninput role (customer/admin): ";

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::cin >> role;

		if (role == "customer")
		{
			std::string com;
			std::cout << "\n\ninput command (show_all/show_clothing_shoes/show_books_author/create_order/view): ";
			std::cin >> com;

			if (com == "show_all")
			{
				std::cout << cust.browse("show all") << "\n\n";
			}

			else if (com == "show_clothing_shoes")
			{
				std::cout << cust.browse("show clothing shoes") << "\n\n";
			}

			else if (com == "show_books_author")
			{
				std::cout << cust.browse("show books author") << "\n\n";
			}
			else if (com == "create_order")
			{
				std::string typ;
				std::string nm;
				std::vector<std::string> buf;
				double p;
				std::cout << "\n\ninput type, name, quantity and details of item: ";
				std::cin >> typ >> nm >> p;

				for (int i = 0; i < 3; i++)
				{
					std::string temp;
					std::cin >> temp;
					buf.push_back(temp);
				}

				cust.create_order(typ, nm, p, buf);
			}

			else if (com == "view")
			{
				cust.view_orders();
			}
		}
		else if (role == "admin")
		{
			std::string com;
			std::cout << "\n\ninput command (add_product/set_quantity): ";
			std::cin >> com;

			if (com == "add_product")
			{
				std::string typ;
				std::string nm;
				std::vector<std::string> buf;
				double p;
				std::cout << "\n\ninput type, name and details of item: ";
				std::cin >> typ >> nm >> p;

				for (int i = 0; i < 3; i++)
				{
					std::string temp;
					std::cin >> temp;
					buf.push_back(temp);
				}

				adm.add_product_to_inventory(typ, nm, p, buf);
			}

			else if (com == "set_quantity")
			{
				std::string typ;
				std::string nm;
				int q;

				std::cout << "\n\ninput type, name, price and quantity: ";

				std::cin >> typ >> nm >> q;

				adm.change_prod_quantity(typ, nm, q);
			}
		}

		syst.init_catalog();
	}

	return 0;
}
