class ElevatorButtons:
    def nextStops(self,currentFloor,currentDirection,buttonsPressed):
        self.currentFloor = currentFloor
        self.currentDirection = currentDirection
        self.buttonsPressed = buttonsPressed
        if currentFloor == 1:
            print('upwards')
        else:
            print('downwards')
        if currentDirection == -1:
            print('downwards')
        else:
            print('upwards')
            return numbers
        
floors = Floors()
print('inside the skyscraper',floors.getcurrentFloor())
