Mat = readtable('SP500_component_stocks.csv');

for i = 1:height(Mat)
    earnings = scrapeEarningsZacks(Mat(i, 2).Symbol{1});
    filename = strcat(Mat(i, 2).Symbol{1}, '.csv');
    disp(filename)
    writecell(earnings, filename);
end

