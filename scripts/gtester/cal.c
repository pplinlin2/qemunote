int cal(int month, int day)
{
	int valid = 0;
	switch(month) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			valid = 0 < day && day <= 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			valid = 0 < day && day <= 30;
			break;
		case 2:
			valid = 0 < day && day <= 29;
			break;
		default:
			break;
	}
	return valid;
}

