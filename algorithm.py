from itertools import permutations, takewhile, chain, product, accumulate, starmap, repeat
from functools import reduce
from random import choice, randint, random, uniform
from operator import ge, sub, and_, truediv, add, mul


def get_distance(p1, p2):
	return sum(starmap(pow, zip(starmap(sub, zip(p1, p2)), repeat(2, len(p1)))))


def make_populations(initial, fitness, predicate):

	def recombination(pair):
		x1, x2 = pair
		d = 0.25

		def get_child():
			alphas = [uniform(-d, 1 + d) for i in range(len(x1))]
			return tuple(starmap(add, zip(x1, starmap(mul, zip(alphas, starmap(sub, zip(x1, x2)))))))
		return (get_child(), get_child())

	def mutation(individual):
		m = 8
		delta = sum([(1 if random() < 1/m else 0) * pow(2, -i) for i in range(m)])
		steps = [(1 if random() < 0.5 else 0) * 0.5 * delta for i in range(len(individual))]
		return tuple(starmap(add, zip(individual, steps)))
	
	def is_good(population):
		return reduce(and_, map(lambda i: fitness(i) < 1, population), True)

	population = initial
	size = len(initial)
	
	while not is_good(population):
		pairs = [(i, choice(population)) for i in population]
		population += filter(predicate, list(map(mutation, chain(*map(recombination, pairs)))))
		population.sort(key=fitness)
		population = population[:size]

	return population


def genetic_algorithm(population, distances, min_lightings):

	size = len(min_lightings)

	def get_lighting(intensities):
		return [sum(starmap(truediv, zip(intensities, sensor))) for sensor in distances]

	def fitness(individual):
		return sum(starmap(sub, zip(get_lighting(individual), min_lightings)))
	
	def predicate(individual):

		def ge_vec(vec1, vec2):
			return reduce(and_, starmap(ge, zip(vec1, vec2)), True)
		return ge_vec(get_lighting(individual), min_lightings) and ge_vec(individual, [0] * len(individual))

	best_population = make_populations(list(population), fitness, predicate)

	print(
		best_population[0],
		fitness(best_population[0])
	)

	return best_population[0]


def generate_intensities(initial_intensities, sensors_coordinates, light_coordinates, min_lightings):

	population = permutations(initial_intensities)
	distances = [tuple(map(lambda sensor: get_distance(sensor, light), sensors_coordinates)) for light in light_coordinates]

	return genetic_algorithm(population, distances, min_lightings)


print(generate_intensities([1,2,3,4], [(1, 1, 1)] * 4, [(0, 0, 0)] * 3, [1, 1, 1]))
