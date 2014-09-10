{
	"targets": [{
		"target_name": "myo",
		"sources": ["src/myo.cpp"],
		"conditions": [
			["OS=='win'", {
				"include_dirs": [
					"include"
				],
				"libraries": [
					"../lib/myo64.lib"
				]
			}]
		]
	}]
}