// An example of watching values with gdb

#include <iostream>
#include <random>

int main()
{
  // Générateur de nombres pseudo-aléatoires (Mersenne Twister) semé par rd
  std::random_device rd;
  std::mt19937 mt{rd()};

  // Distribution uniforme d’entiers dans [0, 100]
  std::uniform_int_distribution<int> dist(0, 100);

  // Run a performing some calculations
  int sum = 0;
  while (true)
  {
    // Tirage d’un seul nombre
    int divisor = dist(mt);
    std::cout << "divisor: " << divisor << "\n";

    // Add some value to sum
    // This has a bug! Our random number generator can return 0!
    int n = (50 / divisor);
    if (divisor == 0)
    {
      throw std::runtime_error("erreur fatale aléatoire");
    }

    sum += n;

    // Break out with some condition
    if (sum > 200) break;
  }

  // Print out sum
  std::cout << sum << '\n';
  return 0;
}
