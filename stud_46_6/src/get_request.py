from json import load

import prettytable

class Phone():
	def __init__(self, _id, pas, man, cost):
		self.id = _id
		self.pas = pas
		self.man = man
		self.cost = cost

	def __str__(self):
		return f'{self.id}, {self.pas}, {self.man}, {self.cost}'

def read_phones():
	document = open('C:\\grape\\guns.json', "r")
	json_format = load(document)
	document.close()
	guns = []
	for gun in json_format:
		guns.append(Phone(gun['id'], gun['passport'], gun['manufacturer'], int((int(gun['range']) - 100) / 7900 * 20 + 1)))
	return guns


result = {'супер-цена': [], 'дешево': [], 'не очень дорого': [], 'средняя цена' : [],
				'дорого': [], 'очень дорого': [], 'неподъемная цена': []}


def get_request(result):
	high_syn = ['дорог', 'высок', 'больш']
	phones = read_phones()
	request = input('Введите запрос: ').lower()
	interval = []
	if 'смартфон' not in request:
		print('В запросе должна идти речь о смартфонах')
		return
	elif 'цен' not in request and 'стоимост' not in request:
		print('В запросе должна идти речь о цене/стоимости смартфона')
		return
	elif "супер-цен" in request:
		interval = result['супер-цена']
	elif "дешев" in request:
		interval = result['дешево']
	elif "средн" in request and "цен" in request:
		interval = result['средняя цена']
	elif (" не " in request or request[:3] == "не ")and "очень" in request:
		flag = 0
		for syn in high_syn:
			if syn in request:
				flag =1
		if flag > 0:
			interval = result['не очень дорого']
	# elif "очень" in request and "высок" in request:
	elif "очень" in request:
		flag = 0
		for syn in high_syn:
			if syn in request:
				flag = 1
		if flag >0:
			interval = result['очень дорого']
	# elif "дорог" in request:
	# 	interval = result['дорого']
	elif ("неподьемн" in request or "неподъемн" in request) and "цен" in request:
		interval = result['неподъемная цена']
	else:
		flag = 0
		for syn in high_syn:
			if syn in request:
				flag = 1
		if flag > 0:
			interval = result['дорого']
		else:
			print('В запросе должна идти речь о каком-то из термов')
			return
	table = prettytable.PrettyTable()
	table.field_names = ['Серийный номер', "Паспорт владельца", "Производитель", "Цена при покупке, десятки тыс. руб."]
	c = 0
	for phone in phones:
		if phone.cost in interval:
			table.add_row(str(phone).split(', '))
			c+=1
		if c>20:
			break
	print(table)