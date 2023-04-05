import prettytable
import matplotlib.pyplot as plt
from get_request import get_request

MARKERS = ['.', 'o', 'v', '^', '<', '>', 's', 'p', 'd']
TYPE = ["-", ":", "--", "go-", "go:", "go--", "go^"]
COLORS = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w']


def print_result(terms, count, minimum, maximum):
	fig = plt.figure(figsize=(10, 7))
	plot = fig.add_subplot()
	costs = list(range(minimum, maximum + 1))
	m = 0
	for key in terms.keys():
		y = [i[1] for i in terms[key]]
		plot.plot(costs, y, '--', label=key, marker=MARKERS[m])
		# plot.plot(costs, y, TYPE[m], label=key, c = COLORS[m])
		m += 1	
	plot.legend()
	plot.grid()
	plt.xlabel('Цена смартфона, десятки тыс. руб.')
	plt.ylabel('μ')
	plt.show()


def main(result, terms, minimum, maximum, count):
	while True:
		choice = 0
		print('''
		1 - Вывести результат опроса
		2 - Задать вопрос

		0 - выход
		''')
		try:
			choice = int(input('Ваш выбор: '))
		except:
			choice = -1
		if choice == 1:
			print_result(terms, count, minimum, maximum)
		elif choice == 2:
			get_request(result)
		elif choice == 0:
			break
		else:
			print('Неверный ввод')

file = open('C:\\grape\\result.csv', 'r', encoding='utf-8')
strings = file.readlines()[1:]
file.close()

for i in range(len(strings)):
	if strings[i][-1] == '\n':
		strings[i] = strings[i][:-1]


respondents = [0] * len(strings)
for i in range(len(respondents)):
	respondents[i] = {'супер-цена': [], 'дешево': [], 'средняя цена' : [], 'не очень дорого': [],
					  'дорого': [], 'очень дорого': [], 'неподъемная цена': []}

i = 0
for string in strings:
	string = string.split(',')[2:]
	j = 0
	for cost in string:
		cost = cost[1:-1].split('-')
		respondents[i][list(respondents[0].keys())[j]] = list(range(int(cost[0]), int(cost[1])+1))
		j += 1
	i += 1

minimum = respondents[0]['супер-цена'][0]
maximum = respondents[0]['неподъемная цена'][-1]
costs = list(range(minimum, maximum + 1))
count = maximum - minimum + 1
terms = {'супер-цена': [0] * count, 'дешево': [0] * count, 'средняя цена': [0] * count,'не очень дорого': [0] * count,
		 'дорого': [0] * count, 'очень дорого': [0] * count, 'неподъемная цена': [0] * count}

for respondent in respondents:
	for key in terms.keys():
		for cost in respondent[key]:
			terms[key][cost - minimum] += 1
	
for key in terms.keys():
	for i in range(count):
		terms[key][i] = [terms[key][i], terms[key][i] / len(respondents)]

result = {'супер-цена': [], 'дешево': [], 'средняя цена' : [],'не очень дорого': [],
				'дорого': [], 'очень дорого': [], 'неподъемная цена': []}

for cost in costs:
	max_v = 0
	max_k = list(terms.keys())[0]
	for key in terms.keys():
		if terms[key][cost - minimum][1] > max_v:
			max_v = terms[key][cost - minimum][1]
			max_k = key
	result[max_k].append(cost)


main(result, terms, minimum, maximum, count)

